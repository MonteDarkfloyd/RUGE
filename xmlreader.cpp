#include <QXmlStreamReader>
#include <QFile>
#include "newsessiondialog.h"
#include "session.h"
#include <QMessageBox>
#include <QDir>

xmlreader::xmlreader()
{
}

void xmlreader::giveErrorPrompt(){
    QString defPath = "predefined/predefinedSessions.xml";
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(NULL, "Attention",
                                  "Predefined session file structure might be corrupted. Would you like to recreate the file?"
                                  "By doing this you will lose all previously saved data!",
                                    QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        xmlWriter xWriter;
        if(xWriter.write())
          QMessageBox::information(NULL,"Info","File recreated successfully. You might need to close and open predefined window to get updated view");
        else
          QMessageBox::warning(NULL,"Error","Error recreating the file, try manually deleting xml file located in predefined folder");
}else{

    }
}



//Read the predefined session variables from xml file
//Put them into list
QList<Session *>  xmlreader::ReadPredefXML(){

    //XMLReader
    QXmlStreamReader Rxml;

    //Variable for filling Session objects
    VariableData vdata;

    //Safety variable for checking XML Document
    safeRead = 1;

    //Initialize list
    QList<Session *> predefList;

    //Path of the file that contains predefined/template sessions
    QDir folder("predefined");
    QString path = folder.filePath("predefinedSessions.xml");
    //("predefined/predefinedSessions.xml");


    //Check if file exists
    if(!fileExists(path)){
        QMessageBox::StandardButton reply;
        //Query user if wants to create file
        reply = QMessageBox::question(NULL, "Attention",
                                      "Predefined session file seems to be missing. Would you like to create it?",
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){

            //Create xmlWriter to create the file in structure
            xmlWriter xWriter;

            //Check if folder exists if not create
            if(!folder.exists()){
            if(!folder.mkdir("predefined"))
                QMessageBox::warning(NULL,"Error","Couldn't create directory. Try creating it manually.");
            }

            //Check if file created
            if(xWriter.write())
            QMessageBox::information(NULL,"Info","File created successfully.");
            else
            QMessageBox::warning(NULL,"Error","Error creating the file, try manually creating xml file located in predefined folder");

        }else{}
    }

        //Open file
        QFile file(path);
            if (!file.open(QFile::ReadOnly | QFile::Text))
        {
        QMessageBox::warning(0, "Error!", "Error opening predefined list file");
        }else{
        //Set XMLStreamReader
        Rxml.setDevice(&file);
         QXmlStreamReader::TokenType token = Rxml.readNext();
         //Find the SessionList Element
         while(token != QXmlStreamReader::StartElement && Rxml.name()!="SessionList" )
        {
               Rxml.readNext();
               if(Rxml.atEnd()){
                   //If there is none possibly corrupt file or structure broken prompt user
                   giveErrorPrompt();
                   safeRead = 0;
               }
        }
        //Find session
        while(!Rxml.atEnd() && Rxml.name() != "Session"){
        if(safeRead == 0){
            giveErrorPrompt();
            break;
        }







            //Create session object and fill in information gotten from XML file
            Session* newSess = new Session();


            //Go to the name field in XML
            while(Rxml.name() != "Name" && safeRead){
            Rxml.readNextStartElement();
            if(Rxml.atEnd())
                break;
            }
            if(Rxml.atEnd())
                break;

            //Set the name for Session object
            newSess->setName(Rxml.readElementText());




            //Go to mac source field in xml file
            while(Rxml.name() != "MacSrc" && safeRead){
            Rxml.readNext();
            if(Rxml.atEnd()){
            giveErrorPrompt();
            safeRead = 0;
            }
            }

            //Set the MAC source for session object
            QString macsrc = Rxml.readElementText();
            rugeVariable macSrcData = vdata.getData("MAC_SRC");
            macSrcData.value = macsrc;

            newSess->addVariable(macSrcData);


            //Go to Destination MAC Address in xml file
            while(Rxml.name() != "MacDst" && safeRead){
            Rxml.readNext();
            if(Rxml.atEnd()){
            giveErrorPrompt();
            safeRead = 0;
            }
            }

            //Add Destination MAC Address to the session data
            QString macdest = Rxml.readElementText();
            rugeVariable macDestData = vdata.getData("MAC_DST");
            macDestData.value = macdest;
            newSess->addVariable(macDestData);


            //Go to ip version field in xml file
            while(Rxml.name() != "Version" &&safeRead){
            Rxml.readNextStartElement();
            if(Rxml.atEnd()){
            giveErrorPrompt();
            safeRead = 0;
            }
            }




            //Add version to the session data
            newSess->setipVersion(Rxml.readElementText());


            //Go to Source ip field in xml file
            while(Rxml.name() != "IPsrc" && safeRead){
            Rxml.readNextStartElement();
            if(Rxml.atEnd()){
            giveErrorPrompt();
            safeRead = 0;
            }
            }


            //Add Source ip information to the session data
            QString ipsrc = Rxml.readElementText();
            rugeVariable ipSrcData = vdata.getData("IP_SRC");
            ipSrcData.value = ipsrc;
            ipSrcData.increment = 1;
            ipSrcData.min = ipSrcData.value;



            //Iterate to IP range max
            while(Rxml.name() != "IPsrcMax" && safeRead){
            Rxml.readNextStartElement();
            if(Rxml.atEnd()){
            giveErrorPrompt();
            safeRead = 0;
            }
            }

             //Add source IP MAX range information to the session data
            ipSrcData.max = Rxml.readElementText();
            newSess->addVariable(ipSrcData);


            //Iterate to Destination IP
            while(Rxml.name() != "IPdst" && safeRead){
            Rxml.readNextStartElement();
            if(Rxml.atEnd()){
            giveErrorPrompt();
            safeRead = 0;
            }
            }


            //Add Destination IP information to the session data
            QString ipdest = Rxml.readElementText();
            rugeVariable ipDestData = vdata.getData("IP_DST");
            ipDestData.value = ipdest;
            ipDestData.increment = 1;
            ipDestData.min = ipDestData.value;




            //Add destination IP MAX range information to the session
            while(Rxml.name() != "IPdstMax" && safeRead){
            Rxml.readNextStartElement();
            if(Rxml.atEnd()){
            giveErrorPrompt();
            safeRead = 0;
            }
            }


            ipDestData.max = Rxml.readElementText();
            newSess->addVariable(ipDestData);


            //Add Protocol information to the session data
            while(Rxml.name() != "Protocol" && safeRead){
            Rxml.readNextStartElement();
            if(Rxml.atEnd()){
            giveErrorPrompt();
            safeRead = 0;
            }
            }

            newSess->setProtocol(Rxml.readElementText());

              //If it is UDP protocol
              if(newSess->getProtocol() == "UDP") {

                  //Go to the source port field in xml
                  while(Rxml.name() != "srcPort" && safeRead){
                  Rxml.readNextStartElement();
                  if(Rxml.atEnd()){
                  giveErrorPrompt();
                  safeRead = 0;
                  }
                  }


                  //Add source port information to the session
                  rugeVariable UDPsrcPORT = vdata.getData("UDP_SRC_PORT");
                  UDPsrcPORT.value = Rxml.readElementText();
                  newSess->addVariable(UDPsrcPORT);

                  //Go to destination port field in xml
                  while(Rxml.name() != "dstPort" && safeRead){
                  Rxml.readNextStartElement();
                  if(Rxml.atEnd()){
                  giveErrorPrompt();
                  safeRead = 0;
                  }
                  }

                  //Add destination port information to the session
                  rugeVariable UDPdstPORT = vdata.getData("UDP_DST_PORT");
                  UDPdstPORT.value = Rxml.readElementText();
                  newSess->addVariable(UDPdstPORT);


              }
              else if(newSess->getProtocol() == "TCP"){
                  //handleTCPvariables(Rxml,newSess);

              }

              //Enter payload data
              while(Rxml.name() != "Payload" && safeRead){
              Rxml.readNextStartElement();
              if(Rxml.atEnd()){
              giveErrorPrompt();
              safeRead = 0;
              }
              }



              newSess->setPayload(Rxml.readElementText());


              //Add the new session to the list
              predefList.append(newSess);



}

}
            return predefList;
}


//Simple function for checking if file exists or not
bool xmlreader::fileExists(QString path){
    QFileInfo sFile(path);
    if (sFile.exists() && sFile.isFile()) {
        return true;
        }else{
        return false;
        }

}

