#include "xmlwriter.h"
#include <QFile>
#include <QtXml/QDomDocument>
#include <QMessageBox>
#include <QErrorMessage>

xmlWriter::xmlWriter()
{ }


/*
 * Create file if needed without any template/predefined session
 *
 */
bool xmlWriter::write(){

   QString defPath = "predefined/predefinedSessions.xml";
   QFile file(defPath);
   if (!file.open(QIODevice::WriteOnly))
   {
     QMessageBox::warning(0, "Error!", "Error opening file");
     return false;
   }
   else
   {
    QXmlStreamWriter xmlwriter;
    xmlwriter.setDevice(&file);

    xmlwriter.setAutoFormatting(true);
    xmlwriter.writeStartDocument();

    //Create root element, sessions will be added into this parent node
    xmlwriter.writeStartElement("SessionList");
    xmlwriter.writeEndElement();




    file.close();


    return true;
   }

   //Structure of XML
   /*
   xmlwriter.writeStartElement("Session");
   xmlwriter.writeTextElement("Name","test2");


   xmlwriter.writeTextElement("MacSrc","");
   xmlwriter.writeTextElement("MacDst","");



   xmlwriter.writeTextElement("Version","IPv4");
   xmlwriter.writeTextElement("IPsrc","192.168.2.1");
   xmlwriter.writeTextElement("IPsrcMax","");


   xmlwriter.writeTextElement("IPdst","256.23.53.212");
   xmlwriter.writeTextElement("IPdstMax","");

   xmlwriter.writeTextElement("Protocol","UDP");
   xmlwriter.writeTextElement("srcPort","66");
   xmlwriter.writeTextElement("dstPort","88");

   xmlwriter.writeTextElement("Payload","test");
   xmlwriter.writeEndElement();

   */


}


//Add new session into predefined list and xml file
bool xmlWriter::add(Session* newSess){

    QDomDocument doc;
    QString defPath = "predefined/predefinedSessions.xml";
    QFile file(defPath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(0, "test","Error opening file");

    }
    // Parse file
    if (!doc.setContent(&file)) {
       QMessageBox::warning(0, "Error","Error in parsing predefined sessions file.");
       file.close();


    }
    file.close();




    //get the root of xml
    QDomElement root = doc.documentElement();


    //Create tag
    QDomElement newSession = doc.createElement(QString("Session"));
    QDomElement newNameTag = doc.createElement("Name");

    //Create text value gotten from the Session object
    QDomText newNameText = doc.createTextNode(newSess->getName());

    //Enter text value into Tag and append it into Session XML Element
    newNameTag.appendChild(newNameText);
    newSession.appendChild(newNameTag);


    //Mac source
    QDomElement newMacSrcTag = doc.createElement("MacSrc");
    QDomText newMacSrcText = doc.createTextNode(newSess->getSrcMAC());
    newMacSrcTag.appendChild(newMacSrcText);
    newSession.appendChild(newMacSrcTag);

    //Mac destination
    QDomElement newMacDstTag = doc.createElement("MacDst");
    QDomText newMacDstText = doc.createTextNode(newSess->getDstMAC());
    newMacDstTag.appendChild(newMacDstText);
    newSession.appendChild(newMacDstTag);

    //Version
    QDomElement newVersionTag = doc.createElement("Version");
    QDomText newVersionText = doc.createTextNode(newSess->getipVersion());
    newVersionTag.appendChild(newVersionText);
    newSession.appendChild(newVersionTag);

    //IP source
    QDomElement newIpsrcTag = doc.createElement("IPsrc");
    QDomText newIPsrcText = doc.createTextNode(newSess->getSrcIP().value);
    newIpsrcTag.appendChild(newIPsrcText);
    newSession.appendChild(newIpsrcTag);

    //IP Source Range MAX
    QDomElement newIpMaxTag = doc.createElement("IPsrcMax");
    QDomText newIpMaxText = doc.createTextNode(newSess->getSrcIP().max); //!!
    newIpMaxTag.appendChild(newIpMaxText);
    newSession.appendChild(newIpMaxTag);

    //IP destination
    QDomElement newIPdstTag = doc.createElement("IPdst");
    QDomText newIPdstText = doc.createTextNode(newSess->getDstIP().value);
    newIPdstTag.appendChild(newIPdstText);
    newSession.appendChild(newIPdstTag);

    //IP destination Range MAX
    QDomElement newIPdstMaxTag = doc.createElement("IPdstMax");
    QDomText newIPdstMaxText = doc.createTextNode(newSess->getDstIP().max);
    newIPdstMaxTag.appendChild(newIPdstMaxText);
    newSession.appendChild(newIPdstMaxTag);


    /*QDomElement newTTLTag = doc.createElement("TTL");
    QDomText newTTLText = doc.createTextNode(newSess->);
    newEthTag.appendChild(newTTLText);
    newSession.appendChild(newTTLTag);*/

    /*QDomElement newOffsetTag = doc.createElement("Offset");
    QDomText newOffsetText = doc.createTextNode();
    newEthTag.appendChild(newOffsetText);
    newSession.appendChild(newOffsetTag);*/

    QDomElement newProtocolTag = doc.createElement("Protocol");
    QDomText newProtocolText = doc.createTextNode(newSess->getProtocol());
    newProtocolTag.appendChild(newProtocolText);
    newSession.appendChild(newProtocolTag);

    //If protocol is UDP
    if(newSess->getProtocol() == "UDP"){
        QDomElement newsrcPortTag = doc.createElement("srcPort");
        QDomText newsrcPortText = doc.createTextNode(newSess->getSrcPort());
        newsrcPortTag.appendChild(newsrcPortText);
        newSession.appendChild(newsrcPortTag);

        QDomElement newdstPortTag = doc.createElement("dstPort");
        QDomText newdstPortText = doc.createTextNode(newSess->getDstPort());
        newdstPortTag.appendChild(newdstPortText);
        newSession.appendChild(newdstPortTag);



    }
    /*else if(newSess->getProtocol() == "TCP"){
        handleTCPVariables();

    }*/

    //Enter Payload
    QDomElement newPayloadTag = doc.createElement("Payload");
    QDomText newPayloadText = doc.createTextNode(newSess->getPayload());
    newPayloadTag.appendChild(newPayloadText);
    newSession.appendChild(newPayloadTag);

    root.appendChild(newSession);


    //Open file for writing
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        QMessageBox::warning(0, "Error", "Cannot edit predefined file.");
        return false;
    }

    //Write into file
    QByteArray xml = doc.toByteArray();
    file.write(xml);
    file.close();

    return true;
}

bool xmlWriter::remove(int row){
    QDomDocument doc;

    //XML file holding predefined list
    QString defPath = "predefined/predefinedSessions.xml";
    QFile file(defPath);

    //Errors
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(0, "Error","Error opening predefined sessions file");
        return false;
    }else if (!doc.setContent(&file)) {
       QMessageBox::warning(0, "Error","Error parsing predefined sessions file");
       file.close();
       return false;

    //success
    }else{
    file.close();

    //Get all session elements
    QDomNodeList nodes = doc.elementsByTagName("Session");
    if(!nodes.isEmpty())
    {
            //Remove corresponding one from stream
            QDomNode node = nodes.at(row);
            node.parentNode().removeChild(node);
    }



    //Write stream into file
    if (!file.open( QIODevice::WriteOnly)) {
        QMessageBox::warning(0, "Error", "Cannot edit predefined file.");
        return false;
    }
    QByteArray xml = doc.toByteArray();
    file.write(xml);
    file.close();

    return true;
   }

}
