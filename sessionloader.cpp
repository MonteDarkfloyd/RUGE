#include "sessionloader.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>

SessionLoader::SessionLoader(QString sessionfile): filename_(sessionfile), session_(0), sessionloaded_(false), message_("")
{
}

SessionLoader::~SessionLoader(){
 if(!sessionloaded_){
     delete session_;
 }

}

// Check if file that has been given in constructor contains valid session.
// Return value: bool, is session valid
// Parameter: error, error message is saved here when return value is false
bool SessionLoader::checkSession(QString &error){
        qDebug() << filename_;
        session_ = new Session();
        QFile xmlFile(filename_);
        if(!xmlFile.exists()){
            error = "Xml file doesn't exist.";
            return false;
        }
        if(!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error = "Failed to open the file for reading.";
            return false;
        }
        QFileInfo name = xmlFile.fileName();
        session_->sessName =name.fileName();
        xml_.setDevice(&xmlFile);

        // Read start of the document.
        xml_.readNext();

        // Read the DTD data from the xml
        xml_.readNext();

        // The file is empty or very short?
        if (xml_.hasError()){
            error = "XML could not be parsed!";
            return false;
        }
        // Check if DTD = RUGESessionData
        if (xml_.dtdName() != "RUGESessionData"){
            error = "XML is not the right type.";
            return false;
        }
        if (xml_.readNext() && xml_.name() == "RUGE_CONFIG"){
            qDebug() << xml_.name();

            // Checking variables starts here
            xml_.readNextStartElement();

            if(xml_.name() != "RUGE_SESSION_VARIABLES"){
                error = "XML could not be parsed!";
                return false;
            }

            xml_.readNextStartElement();
            qDebug() << "Start variable parsing";
            qDebug() << xml_.name();
            while(xml_.name() == "RUGE_SESSION_VARIABLE" && !xml_.atEnd()){

                // check variable
                if(!readVariable()){
                    error = "XML could not be parsed!\nUnknown variable.";
                    return false;
                }

                xml_.readNextStartElement();
                if(xml_.isEndElement()){
                    xml_.readNextStartElement();
                }
                qDebug() << xml_.name() << "  Loop1";
             }

             qDebug() << xml_.name() << "  Loop1 end";
             if(!checkStates()){
                error = "XML could not be parsed!\nNo/unknown states.";
                return false;
             }
             qDebug() << "Control message start";
             if(!checkControlMessage()){
                 error = "XML could not be parsed!\nUnknown protocols or unknown control message headers.";
                 return false;
             }
             qDebug() << "Payload start";
             if(!buildPayload()){
                 error = "Packet or payload error in XML.";
                 return false;
             }
             qDebug() << "Timeout start";
             if(!checkTimeout()){
                 error = "XML couldn't be parsed.";
                 return false;
             }
             qDebug() << "Flow start";
             if(!checkSessionflow()){
                 error = "XML couldn't be parsed.";
                 return false;
             }
             return true;

        }
        error = "XML could not be parsed!";
        return false;
}
Session* SessionLoader::loadSession(){
    sessionloaded_ = true;
    return session_;
}


// Reads variable from xml_ and sets the variable value to the session_.
// Return value: bool, returns false if variable was unknown, otherwise returns true.
bool SessionLoader::readVariable(){

    // Variable doesn't have VARIABLE attribute = error
    if(!xml_.attributes().hasAttribute("VARIABLE")){
        qDebug() << "No Variable" << xml_.isEndElement();
       return false;
    }
    else{
        qDebug() << "Variable found";
        QString value = "";
        value = xml_.attributes().value("VARIABLE").toString();
        qDebug() << value;
        // TODO check if mac and IP are valid
        // Check what variable is this.
        if(value == "MAC_SRC"){

            // No default aka no value?
           if(!xml_.attributes().hasAttribute("DEFAULT")){
                       qDebug() << "No Default";
               return false;
           }
           session_->srcMAC = xml_.attributes().value("DEFAULT").toString();           
             qDebug() << session_->srcMAC;
           return true;


        }

        else if(value == "MAC_DST"){
            // No default aka no value?
           if(!xml_.attributes().hasAttribute("DEFAULT")){
               qDebug() << "No Default";
               return false;
           }
           session_->dstMAC = xml_.attributes().value("DEFAULT").toString();
           return true;
        }

        else if(value == "IP_SRC"){
            // No default aka no value?
           if(!xml_.attributes().hasAttribute("DEFAULT")){
               qDebug() << "No Default";
               return false;
           }
           session_->srcIP = xml_.attributes().value("DEFAULT").toString();

           if(xml_.attributes().value("MAX").toString() != "" && xml_.attributes().value("INCREMENT").toString() == "1"){
                       session_->dstIPmax = xml_.attributes().value("MAX").toString();
           }

           return true;
        }

        else if(value == "IP_DST"){
            // No default aka no value?
           if(!xml_.attributes().hasAttribute("DEFAULT")){
               qDebug() << "No Default";
               return false;
           }

           session_->dstIP = xml_.attributes().value("DEFAULT").toString();
           if(xml_.attributes().value("MAX").toString() != "" && xml_.attributes().value("INCREMENT").toString() == "1"){
                       session_->srcIPmax = xml_.attributes().value("MAX").toString();
           }

           return true;
        }
        else if(value == "IP_TTL"){
            // No default aka no value?
           if(!xml_.attributes().hasAttribute("DEFAULT")){
               qDebug() << "No Default";
               return false;
           }
           session_->ttl = xml_.attributes().value("DEFAULT").toString();
           return true;
        }
        else if(value == "UDP_SRC_PORT"){
            // No default aka no value?
           if(!xml_.attributes().hasAttribute("DEFAULT")){
               qDebug() << "No Default";
               return false;
           }
           session_->udp->srcPort = xml_.attributes().value("DEFAULT").toString();
           return true;
        }
        else if(value == "UDP_DST_PORT"){
            // No default aka no value?
           if(!xml_.attributes().hasAttribute("DEFAULT")){
               qDebug() << "No Default";
               return false;
           }
           session_->udp->dstPort = xml_.attributes().value("DEFAULT").toString();
           return true;
        }
        else if(value == "UDP_LENGTH"){
            // No default aka no value?
           if(!xml_.attributes().hasAttribute("DEFAULT")){
               qDebug() << "No Default";
               return false;
           }
           session_->udp->length = xml_.attributes().value("DEFAULT").toString();
           return true;
        }
        else{
            return false;
        }


    }
    return false;
}
// Search xml_ for RUGE_SESSION_STATES and checks if they are legal.
// Return value: bool, was legal states found, false if not.
bool SessionLoader::checkStates(){

    bool states = false;
    while(!states){

        // Are we at the end of the file?
        if(xml_.atEnd()){
          return false;
        }

        if(xml_.name() == "RUGE_SESSION_STATES"){
            xml_.readNextStartElement();

            // No states are found?
            if(xml_.name() != "RUGE_SESSION_STATE"){
                return false;
            }
            else{
                while(xml_.name() == "RUGE_SESSION_STATE"){

                    // We are at the end of file
                    if(xml_.atEnd()){
                      return false;
                    }

                    qDebug() << xml_.name() << "  Loop state start";
                    qDebug() << xml_.attributes().value("STATE").toString();

                    // Check if state is named START or START 2. No other states are known.
                    if(xml_.attributes().value("STATE").toString() != "START" && xml_.attributes().value("STATE").toString() != "START 2"){
                        return false;
                    }
                    xml_.readNextStartElement();
                    if(xml_.isEndElement()){
                       xml_.readNextStartElement();
                    }
                }
                // Both states were found, break from the loop.
                qDebug() << xml_.name() << "  Loop state end";
                states = true;
            }
        }
        else{
            xml_.readNext();
        }

    }
    return true;
}

bool SessionLoader::checkControlMessage(){
    while(!xml_.atEnd()){
        if(xml_.name() == "RUGE_SESSION_CONTROL_MESSAGES"){
            xml_.readNextStartElement();
            qDebug() << xml_.name();
            // Error because broken XML
            if(xml_.name() != "RUGE_SESSION_CONTROL_MESSAGE"){
                qDebug() << xml_.name();
                return false;
            }
            else{
                if(!xml_.attributes().hasAttribute("NAME")){
                    qDebug() << "No name for control message";
                   return false;
                }
                else{
                    message_ = xml_.attributes().value("NAME").toString();
                    xml_.readNextStartElement();
                    if(xml_.name() != "RUGE_SESSION_CONTROL_MESSAGE_HEADERS_STACK"){
                        qDebug() << xml_.name();
                        return false;
                    }
                    else{
                        // Ethernet protocol
                        if(xml_.attributes().value("Protocol_0").toString() != "Ethernet_II"){
                            return false;
                        }
                        // Internet protocol, now only IPv4
                        if(xml_.attributes().value("Protocol_1").toString() == "IPv4"){
                            session_->ipVersion = "IPv4";
                        }
                        else{
                            return false;
                        }
                        // Internet suite protocol
                        // Add more if necessary, now UDP only
                        if(xml_.attributes().value("Protocol_2").toString() == "UDP"){
                            session_->protocol = "UDP";
                        }
                        else {
                            return false;
                        }
                    }
                }
                // Start reading MESSAGE HEADER VARIABLES
                // These must be found.

                bool rugetotallenght = false;
                bool udplenght = false;
                bool udpcheck = false;
                qDebug() << xml_.name();

                while(!xml_.atEnd()){
                    xml_.readNextStartElement();
                    if(xml_.name() == "RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES"){
                        while(xml_.name() == "RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES"){
                            if(xml_.attributes().value("OFFSET").toString() == "16"){
                                if(xml_.attributes().value("VARIABLE").toString() != "ruge_protocol_variable_ipv4_total_length"){
                                    return false;
                                }
                                rugetotallenght = true;
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "38"){
                                if(xml_.attributes().value("VARIABLE").toString() != "ruge_protocol_variable_udp_length"){
                                    return false;
                                }
                                udplenght = true;
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "40"){
                                if(xml_.attributes().value("VARIABLE").toString() != "ruge_protocol_variable_udp_csum"){
                                    return false;
                                }
                                udpcheck = true;
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "0"){
                                if(xml_.attributes().value("VARIABLE").toString() != "MAC_DST"){
                                    return false;
                                }
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "6"){
                                if(xml_.attributes().value("VARIABLE").toString() != "MAC_SRC"){
                                    return false;
                                }
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "26"){
                                if(xml_.attributes().value("VARIABLE").toString() != "IP_SRC"){
                                    return false;
                                }
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "30"){
                                if(xml_.attributes().value("VARIABLE").toString() != "IP_DST"){
                                    return false;
                                }
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "22"){
                                if(xml_.attributes().value("VARIABLE").toString() != "IP_TTL"){
                                    return false;
                                }
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "34"){
                                if(xml_.attributes().value("VARIABLE").toString() != "UDP_SRC_PORT"){
                                    return false;
                                }
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "36"){
                                if(xml_.attributes().value("VARIABLE").toString() != "UDP_DST_PORT"){
                                    return false;
                                }
                            }
                            else if(xml_.attributes().value("OFFSET").toString() == "38"){
                                if(xml_.attributes().value("VARIABLE").toString() != "UDP_LENGTH"){
                                    return false;
                                }
                            }
                        qDebug() << xml_.name();
                        xml_.readNextStartElement();
                        if(xml_.isEndElement()){
                            xml_.readNextStartElement();
                        }
                        }
                        if(!rugetotallenght || !udplenght || !udpcheck){
                            return false;
                        }
                        qDebug() << "Control message header loop ended";
                        return true;
                    }
                }
            }
        }
        else{
            xml_.readNext();
        }
    }
    return false;
}


// Checks the packet data and builds payload.
bool SessionLoader::buildPayload(){
    int lenght = 0;
    while(!xml_.atEnd()){
        qDebug() << xml_.name();
        if(xml_.name() == "RUGE_SESSION_CONTROL_MESSAGE_HEADER_REFERENCE_DATA"){
            if(xml_.attributes().hasAttribute("LENGTH")){
                bool ok = false;
                lenght = xml_.attributes().value("LENGTH").toInt(&ok);
                qDebug() << lenght;
                if(!ok || lenght < 84 || (lenght % 2 != 0)){
                    qDebug() << "Lenght error";
                    return false;
                }

                if(!xml_.attributes().value("VALUE").startsWith("11112233445522112233445508004500001c0000400040110000a228e880a228e8812125215300080000")){
                    qDebug() << "Packet error";
                    return false;
                }
                // There is no payload in this packet
                if(lenght == 84){
                    return true;
                }
                else{
                    // Create payload.
                    QString payloadraw = xml_.attributes().value("VALUE").toString().remove(0,84);
                    QByteArray payload = QByteArray::fromHex(payloadraw.toUtf8());
                    payloadraw = payload.data();
                    qDebug() << payloadraw;
                    session_->payload = payloadraw;
                    return true;
                }
            }
            else{
                return false;
            }
        }
        xml_.readNext();
    }
    return false;
}

bool SessionLoader::checkTimeout(){
     while(!xml_.atEnd()){
         if(xml_.name() == "RUGE_SESSION_INTERFACES"){
             xml_.readNextStartElement();
             if(xml_.atEnd()){
                 return false;
             }
             if(xml_.name() != "RUGE_SESSION_INTERFACE" || xml_.attributes().value("STREAM_OUTPUT_PORT").toString() != "" ||
                     xml_.attributes().value("MESSAGE_OUTPUT_PORT").toString() != "" || xml_.attributes().value("NAME").toString() != "Timeout"){
                return false;
             }
             xml_.readNextStartElement();
             if(xml_.atEnd()){
                 return false;
             }
             if(xml_.name() != "RUGE_SESSION_FLOW" || xml_.attributes().value("STATEFUL").toString() != "1"){
                return false;
             }
             xml_.readNextStartElement();
             if(xml_.atEnd()){
                 return false;
             }
             // Check time out data.
             if(xml_.name() != "RUGE_SESSION_FLOW_ITEM" || xml_.attributes().value("ITEM").toString() != "TIMEOUT" ||
                     xml_.attributes().value("OFFSET").toString() != "0" || xml_.attributes().value("COLUMN_INDEX").toString() != "0" ||
                     xml_.attributes().value("TYPE").toString() != "STATE" || xml_.attributes().value("ROW_INDEX").toString() != "1"){
                return false;
             }
             xml_.readNext();
             if(xml_.atEnd()){
                 return false;
             }
             return true;
         }
     xml_.readNext();
     }
    return false;
}

bool SessionLoader::checkSessionflow(){
    bool flowfound = false;
    while(!xml_.atEnd()){
        qDebug() << xml_.name() << "x" << xml_.tokenType();

        if(xml_.name() == "RUGE_SESSION_INTERFACE" && !xml_.isEndElement()){
            if(flowfound){
                return false;
            }
            qDebug() << "Start";
            qDebug() << xml_.name() << "0";

            if(xml_.attributes().value("SESSION_TIMEOUT_VALUE").toString() != "1" || xml_.attributes().value("CONNECTION").toString() != "" ||
                    xml_.attributes().value("STREAM_OUTPUT_PORT").toString() != "0" || xml_.attributes().value("SESSION_IS_TIMEOUT_ENABLED").toString() != "FALSE" ||
                    xml_.attributes().value("MESSAGE_OUTPUT_PORT").toString() != "0" || xml_.attributes().value("NAME").toString() != "Procedure 1"){
               return false;
            }

            xml_.readNextStartElement();
            if(xml_.atEnd()){
                return false;
            }

            qDebug() << xml_.name() << "1";
            if(xml_.name() != "RUGE_SESSION_FLOW" || xml_.attributes().value("STATEFUL").toString() != "1"){
               return false;
            }
            xml_.readNextStartElement();
            if(xml_.atEnd()){
                return false;
            }

            qDebug() << xml_.name() << "2";
            // Check start flow out data.
            if(xml_.name() != "RUGE_SESSION_FLOW_ITEM" || xml_.attributes().value("ITEM").toString() != "START" ||
                    xml_.attributes().value("OFFSET").toString() != "0" || xml_.attributes().value("COLUMN_INDEX").toString() != "0" ||
                    xml_.attributes().value("TYPE").toString() != "STATE" || xml_.attributes().value("ROW_INDEX").toString() != "1"){
               return false;
            }
            xml_.readNextStartElement();
            if(xml_.atEnd()){
                return false;
            }
            if(xml_.isEndElement()){
                xml_.readNextStartElement();
            }

            qDebug() << message_ << xml_.name() <<xml_.attributes().value("ITEM").toString() ;
            // Check packet flow out data.
            if(xml_.name() != "RUGE_SESSION_FLOW_ITEM" || xml_.attributes().value("ITEM").toString() != message_ ||
                    xml_.attributes().value("OFFSET").toString() != "1" || xml_.attributes().value("COLUMN_INDEX").toString() != "1" ||
                    xml_.attributes().value("TYPE").toString() != "MESSAGE" || xml_.attributes().value("ROW_INDEX").toString() != "1"){
               return false;
            }
            qDebug() << "Almost success !";
            xml_.readNextStartElement();
            if(xml_.atEnd()){
                return false;
            }
            if(xml_.isEndElement()){
                xml_.readNextStartElement();
            }
            // Check start again flow out data.
            if(xml_.name() != "RUGE_SESSION_FLOW_ITEM" || xml_.attributes().value("ITEM").toString() != "START" ||
                    xml_.attributes().value("OFFSET").toString() != "0" || xml_.attributes().value("COLUMN_INDEX").toString() != "2" ||
                    xml_.attributes().value("TYPE").toString() != "STATE" || xml_.attributes().value("ROW_INDEX").toString() != "1"){
               return false;
            }
            flowfound = true;
            qDebug() << "Success !";
        }
    xml_.readNextStartElement();
    }
    if(flowfound){
        return true;
    }
   return false;
}
