#include "xmlwriter.h"
#include <QFile>
#include <QtXml/QDomDocument>
#include <QMessageBox>
#include <QErrorMessage>

xmlWriter::xmlWriter()
{ }




void xmlWriter::write(){
    QFile file("test3.xml");
   if (!file.open(QIODevice::WriteOnly))
   {
     QMessageBox::warning(0, "Error!", "Error opening file");
   }
   else
   {
    QXmlStreamWriter xmlwriter;
    xmlwriter.setDevice(&file);

    xmlwriter.setAutoFormatting(true);
    xmlwriter.writeStartDocument();
    xmlwriter.writeStartElement("SessionList");
    /*xmlwriter.writeStartElement("Session");
    xmlwriter.writeTextElement("Name","test2");

    xmlwriter.writeTextElement("Ethernet","eth2");

    xmlwriter.writeTextElement("MacSrc","");
    xmlwriter.writeTextElement("MacDst","");



    xmlwriter.writeTextElement("Version","IPv4");
    xmlwriter.writeTextElement("IPsrc","192.168.2.1");
    xmlwriter.writeTextElement("IncrementSrc","4");


    xmlwriter.writeTextElement("IPdst","256.23.53.212");
    xmlwriter.writeTextElement("IncrementDst","");

    xmlwriter.writeTextElement("TTL","");
    xmlwriter.writeTextElement("Offset","");

    xmlwriter.writeTextElement("Protocol","TCP");
    xmlwriter.writeTextElement("srcPort","66");
    xmlwriter.writeTextElement("dstPort","88");
    xmlwriter.writeTextElement("UrgentPointer","");
    xmlwriter.writeTextElement("SeqNo","");
    xmlwriter.writeTextElement("AckNo","");
    xmlwriter.writeTextElement("URG",0);
    xmlwriter.writeTextElement("ACK",0);
    xmlwriter.writeTextElement("SYN",0);
    xmlwriter.writeTextElement("RST",0);
    xmlwriter.writeTextElement("PSH",0);
    xmlwriter.writeTextElement("FIN","0");

    xmlwriter.writeTextElement("Payload","test");
    xmlwriter.writeEndElement();
*/


    xmlwriter.writeEndElement();

    QMessageBox::warning(0, "test","yazdm");

    file.close();
   }

}


//Add new session into predefined list and xml file
void xmlWriter::add(Session* newSess){
    QDomDocument doc;
    
    //XML file holding predefined list
    QFile file("test3.xml");
    
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(0, "test","hata");
        return ;
    }
    // Parse file
    if (!doc.setContent(&file)) {
       QMessageBox::warning(0, "test","err2");
       file.close();

    }
    file.close();

    // Modify file add new session tags after last one


    //get the root of xml
    QDomElement root = doc.documentElement();

    //test variables
    //QString t2 = root.tagName();
    //QMessageBox::warning(0, "test", t2);


    //Create tag and add the value for Session name
    QDomElement newSession = doc.createElement(QString("Session"));
    QDomElement newNameTag = doc.createElement("Name");
    QDomText newNameText = doc.createTextNode(newSess->getName());
    newNameTag.appendChild(newNameText);
    newSession.appendChild(newNameTag);

    //Ethernet type
    QDomElement newEthTag = doc.createElement("Ethernet");
    QDomText newEthText = doc.createTextNode("eth2");
    newEthTag.appendChild(newEthText);
    newSession.appendChild(newEthTag);


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
    QDomText newIPsrcText = doc.createTextNode(newSess->getSrcIP());
    newIpsrcTag.appendChild(newIPsrcText);
    newSession.appendChild(newIpsrcTag);

    /*QDomElement newIncSrcTag = doc.createElement("IncrementSrc");
    QDomText newIncSrcText = doc.createTextNode(newSess->);
    newEthTag.appendChild(newIncSrcText);
    newSession.appendChild(newIncSrcTag);*/

    //IP destination
    QDomElement newIPdstTag = doc.createElement("IPdst");
    QDomText newIPdstText = doc.createTextNode(newSess->getDstIP());
    newIPdstTag.appendChild(newIPdstText);
    newSession.appendChild(newIPdstTag);

    /*QDomElement newIncDstTag = doc.createElement("IncrementDst");
    QDomText newIncDstText = doc.createTextNode(newSess->incrementDst);
    newEthTag.appendChild(newIncDstText);
    newSession.appendChild(newIncDstTag);*/


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

        QDomElement newsrcPortTag = doc.createElement("srcPort");
        QDomText newsrcPortText = doc.createTextNode();
        newsrcPortTag.appendChild(newsrcPortText);
        newSession.appendChild(newsrcPortTag);

        QDomElement newdstPortTag = doc.createElement("dstPort");
        QDomText newdstPortText = doc.createTextNode();
        newdstPortTag.appendChild(newdstPortText);
        newSession.appendChild(newdstPortTag);

        QDomElement newUrgentPointerTag = doc.createElement("UrgentPointer");
        QDomText newUrgentPointerText = doc.createTextNode();
        newUrgentPointerTag.appendChild(newUrgentPointerText);
        newSession.appendChild(newUrgentPointerTag);

        QDomElement newSeqNoTag = doc.createElement("SeqNo");
        QDomText newSeqNoText = doc.createTextNode();
        newSeqNoTag.appendChild(newSeqNoText);
        newSession.appendChild(newSeqNoTag);

        QDomElement newAckNoTag = doc.createElement("AckNo");
        QDomText newAckNoText = doc.createTextNode();
        newAckNoTag.appendChild(newAckNoText);
        newSession.appendChild(newAckNoTag);

        QDomElement newURGTag = doc.createElement("URG");
        QDomText newURGText = doc.createTextNode();
        newURGTag.appendChild(newURGText);
        newSession.appendChild(newURGTag);

        QDomElement newACKTag = doc.createElement("ACK");
        QDomText newACKText = doc.createTextNode();
        newACKTag.appendChild(newACKText);
        newSession.appendChild(newACKTag);

        QDomElement newSYNTag = doc.createElement("SYN");
        QDomText newSYNText = doc.createTextNode();
        newSYNTag.appendChild(newSYNText);
        newSession.appendChild(newSYNTag);

        QDomElement newRSTTag = doc.createElement("RST");
        QDomText newRSTText = doc.createTextNode();
        newRSTTag.appendChild(newRSTText);
        newSession.appendChild(newRSTTag);

        QDomElement newPSHTag = doc.createElement("PSH");
        QDomText newPSHText = doc.createTextNode();
        newPSHTag.appendChild(newPSHText);
        newSession.appendChild(newPSHTag);

        QDomElement newFINTag = doc.createElement("FIN");
        QDomText newFINText = doc.createTextNode();
        newFINTag.appendChild(newFINText);
        newSession.appendChild(newFINTag);

    }*/

    QDomElement newPayloadTag = doc.createElement("Payload");
    QDomText newPayloadText = doc.createTextNode(newSess->getPayload());
    newPayloadTag.appendChild(newPayloadText);
    newSession.appendChild(newPayloadTag);

    root.appendChild(newSession);


    //QMessageBox::warning(0, "test","tt");
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        QMessageBox::warning(0, "Error", "Cannot edit predefined file.");
        return;
    }
    QByteArray xml = doc.toByteArray();
    file.write(xml);
    file.close();


}
