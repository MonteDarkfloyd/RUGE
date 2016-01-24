#include "sessionloader.h"
#include "variabledata.h"
#include <QFile>
#include <QFileInfo>

SessionLoader::SessionLoader(QString sessionfile): filename_(sessionfile), session_(0), sessionloaded_(false), message_("")
{
}

SessionLoader::~SessionLoader(){

 // Session has never been loaded from here so delete it.
 if(!sessionloaded_){
     delete session_;
 }

}

// Check if file that has been given in constructor contains valid session.
// Return value: bool, is session valid
// Parameter: error, error message is saved here when return value is false
bool SessionLoader::checkSession(QString &error){
        session_ = new Session();
        QFile xmlFile(filename_);

        // File doesn't exist.
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
        session_->setName(name.fileName());
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

            // Checking the start variables begins here
            xml_.readNextStartElement();

            if(xml_.name() != "RUGE_SESSION_VARIABLES"){
                error = "XML could not be parsed!";
                return false;
            }

            xml_.readNextStartElement();

            // Check the variables found from xml
            while(xml_.name() == "RUGE_SESSION_VARIABLE" && !xml_.atEnd()){

                // Check one variable
                if(!readVariable()){
                    error = "XML could not be parsed!\nUnknown variable.";
                    return false;
                }

                xml_.readNextStartElement();

                if(xml_.isEndElement()){
                    xml_.readNextStartElement();
                }

             }

             // Check the State part of xml
             if(!checkStates()){
                error = "XML could not be parsed!\nNo/unknown states.";
                return false;
             }

             // Check the controlmessage part of xml
             if(!checkControlMessage()){
                 error = "XML could not be parsed!\nUnknown protocols or unknown control message headers.";
                 return false;
             }

             // Check and build the payload
             if(!buildPayload()){
                 error = "Packet or payload error in XML.";
                 return false;
             }

             // Checck the timeout part of xml
             if(!checkTimeout()){
                 error = "XML couldn't be parsed.";
                 return false;
             }

             // Check the session flow
             if(!checkSessionflow()){
                 error = "XML couldn't be parsed.";
                 return false;
             }

             // All checks complete.
             return true;

        }
        error = "XML could not be parsed!";
        return false;
}


Session* SessionLoader::loadSession(){
    // Set sessionloaded true so that destructor doesn't delete session.
    sessionloaded_ = true;
    return session_;
}


// Reads variable from xml_ and sets the variable value to the session_.
// Return value: bool, returns false if variable was unknown, otherwise returns true.
bool SessionLoader::readVariable(){

    // Variable doesn't have VARIABLE attribute = error
    if(!xml_.attributes().hasAttribute("VARIABLE")){
       return false;
    }
    else{

        // Load the information from the found variable. Offset is not found here
        // so it is set to 0
        //  name, value, min, max, type, loopIncrement, increment, size, offset
        rugeVariable loadVar = {xml_.attributes().value("VARIABLE").toString(),
                               xml_.attributes().value("DEFAULT").toString(),
                               xml_.attributes().value("MIN").toString(),
                               xml_.attributes().value("MAX").toString(),
                               xml_.attributes().value("TYPE").toString(),
                               xml_.attributes().value("LOOP_INCREMENT").toInt(),
                               xml_.attributes().value("INCREMENT").toInt(),
                               xml_.attributes().value("SIZE").toInt(),
                               0};
        VariableData checker;
        // Check if the variable is legal and add offset if it was.
        if(!checker.checkVariable(loadVar)){
            return false;
        }

        session_->addVariable(loadVar);
        return true;
    } 
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
                states = true;
            }
        }
        else{
            xml_.readNext();
        }

    }
    return true;
}

// Search xml_ for RUGE_SESSION_CONTROL_MESSAGES and checks if they are legal.
// Return value: bool, was legal states found, false if not.
bool SessionLoader::checkControlMessage(){
    while(!xml_.atEnd()){
        if(xml_.name() == "RUGE_SESSION_CONTROL_MESSAGES"){

            xml_.readNextStartElement();

            // Error because broken XML
            if(xml_.name() != "RUGE_SESSION_CONTROL_MESSAGE"){

                return false;
            }
            else{
                // Has no name attribute
                if(!xml_.attributes().hasAttribute("NAME")){
                   return false;
                }

                else{
                    message_ = xml_.attributes().value("NAME").toString();
                    xml_.readNextStartElement();
                    if(xml_.name() != "RUGE_SESSION_CONTROL_MESSAGE_HEADERS_STACK"){

                        return false;
                    }
                    else{
                        // Ethernet protocol
                        if(xml_.attributes().value("Protocol_0").toString() != "Ethernet_II"){
                            return false;
                        }
                        // Internet protocol, now only IPv4
                        if(xml_.attributes().value("Protocol_1").toString() == "IPv4"){
                            session_->setipVersion("IPv4");
                        }
                        else{
                            return false;
                        }
                        // Internet suite protocol
                        // Add more if necessary, now UDP only
                        if(xml_.attributes().value("Protocol_2").toString() == "UDP"){
                            session_->setProtocol("UDP");
                        }
                        else {
                            return false;
                        }
                    }
                }
                // Start reading MESSAGE HEADER VARIABLES
                // These 3 must be found for now. We ignore others
                // for now.

                bool rugetotallenght = false;
                bool udplenght = false;
                bool udpcheck = false;

                // As long as the xml is not at the end try to find them.
                while(!xml_.atEnd()){

                    xml_.readNextStartElement();

                    if(xml_.name() == "RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES"){

                        // RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES found, go throught them.
                        while(xml_.name() == "RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES"){

                            // Check if variables data is correct.
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

                            xml_.readNextStartElement();
                                if(xml_.isEndElement()){
                                    xml_.readNextStartElement();
                                }
                        }

                        // If the 3 were not found, xml is not valid.
                        if(!rugetotallenght || !udplenght || !udpcheck){
                            return false;
                        }

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
    int length = 0;
    while(!xml_.atEnd()){

        if(xml_.name() == "RUGE_SESSION_CONTROL_MESSAGE_HEADER_REFERENCE_DATA"){

            // Check lenght.
            if(xml_.attributes().hasAttribute("LENGTH")){
                bool ok = false;
                // Is lenght a number?
                length = xml_.attributes().value("LENGTH").toInt(&ok);

                // Length is over 84 and it can be divined by 2
                if(!ok || length < 84 || (length % 2 != 0)){

                    return false;
                }

                // Check the Ruge default packet
                if(!xml_.attributes().value("VALUE").startsWith("11112233445522112233445508004500001c0000400040110000a228e880a228e8812125215300080000")){

                    return false;
                }
                // There is no payload in this packet
                if(length == 84){
                    return true;
                }
                else{
                    // Create payload.
                    QString payloadraw = xml_.attributes().value("VALUE").toString().remove(0,84);
                    QByteArray payload = QByteArray::fromHex(payloadraw.toUtf8());
                    payloadraw = payload.data();

                    session_->setPayload(payloadraw);
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

// Check if there is time out part in the xml
bool SessionLoader::checkTimeout(){
     while(!xml_.atEnd()){

         // The part where the time out starts is found.
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
    // The time out was not found.
    return false;
}

// Checks session flow of the xml. That we send only one packet.
// The flow should be: START -> message_ ->START
// If xml has something after this we ignore it.
bool SessionLoader::checkSessionflow(){
    bool flowfound = false;
    while(!xml_.atEnd()){

        if(xml_.name() == "RUGE_SESSION_INTERFACE" && !xml_.isEndElement()){
            if(flowfound){
                return false;
            }

            if(xml_.attributes().value("SESSION_TIMEOUT_VALUE").toString() != "1" || xml_.attributes().value("CONNECTION").toString() != "" ||
                    xml_.attributes().value("STREAM_OUTPUT_PORT").toString() != "0" || xml_.attributes().value("SESSION_IS_TIMEOUT_ENABLED").toString() != "FALSE" ||
                    xml_.attributes().value("MESSAGE_OUTPUT_PORT").toString() != "0" || xml_.attributes().value("NAME").toString() != "Procedure 1"){
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

            // Check packet flow out data. Attribute ITEM must be equal to message_
            if(xml_.name() != "RUGE_SESSION_FLOW_ITEM" || xml_.attributes().value("ITEM").toString() != message_ ||
                    xml_.attributes().value("OFFSET").toString() != "1" || xml_.attributes().value("COLUMN_INDEX").toString() != "1" ||
                    xml_.attributes().value("TYPE").toString() != "MESSAGE" || xml_.attributes().value("ROW_INDEX").toString() != "1"){
               return false;
            }
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
        }
    xml_.readNextStartElement();
    }
    if(flowfound){
        return true;
    }
   return false;
}
