#include "xmlreader.h"
#include <QXmlStreamReader>
#include <QFile>
#include "newsessiondialog.h"
#include "session.h"
#include <QMessageBox>

xmlreader::xmlreader()
{

}



//Read the predefined session variables from xml file
//Put them into list
void xmlreader::Read(NewSessionDialog* window){

    QXmlStreamReader Rxml;


        //Predefined xml file 
        QFile file("test3.xml");
            if (!file.open(QFile::ReadOnly | QFile::Text))
        {

    QMessageBox::warning(0, "Error!", "Error opening predefined list file");
        }

        Rxml.setDevice(&file);
         QXmlStreamReader::TokenType token = Rxml.readNext();
         while(token != QXmlStreamReader::StartElement && Rxml.name() != "SessionList" ){
               QXmlStreamReader::TokenType token = Rxml.readNext();
}
        while(!Rxml.atEnd()){


            while(Rxml.name() != "Session" && !Rxml.atEnd())
            Rxml.readNextStartElement();


            Session* newSess = new Session();
            Rxml.readNextStartElement();
            t = Rxml.name().toString();
            //QMessageBox::warning(0, "test", t);



            newSess->setName(Rxml.readElementText());





            while(Rxml.name() != "MacSrc"){
            Rxml.readNext();}

                rugeVariable MacSrc = {"MAC_SRC","","","","",0,0,0,0};

                newSess->addVariable(MacSrc);


            while(Rxml.name() != "MacDst")
            Rxml.readNext();

            newSess->dstMAC = Rxml.readElementText();

            while(Rxml.name() != "Version")
            Rxml.readNextStartElement();

            newSess->ipVersion = Rxml.readElementText();


            while(Rxml.name() != "IPsrc")
            Rxml.readNextStartElement();

            newSess->srcIP = Rxml.readElementText();


            while(Rxml.name() != "Increment")
            Rxml.readNextStartElement();

            newSess->incrementSrc = Rxml.readElementText();


            while(Rxml.name() != "IPdst")
            Rxml.readNextStartElement();

            newSess->dstIP = Rxml.readElementText();

          /*  while(Rxml.name() != "Increment")
            Rxml.readNextStartElement();

            newSess->incrementDst = Rxml.readElementText();

            while(Rxml.name() != "TTL")
            Rxml.readNextStartElement();

            newSess->TTL = Rxml.readElementText();


            while(Rxml.name() != "Offset")
            Rxml.readNextStartElement();

            newSess->IPOffset = Rxml.readElementText();*/

            while(Rxml.name() != "Protocol")
            Rxml.readNextStartElement();

            newSess->setProtocol(Rxml.readElementText());

              if(newSess->getProtocol() == "UDP") {
                  //handleUDPvariables(Rxml,newSess);

              }
              else if(newSess->getProtocol() == "TCP"){
                  //handleTCPvariables(Rxml,newSess);

              }


              while(Rxml.name() != "Payload")
              Rxml.readNextStartElement();

              newSess->setPayload(Rxml.readNextStartElement());

              
              //Add the new session to list view
              window->predefList.append(newSess);
}   
        
        

}


