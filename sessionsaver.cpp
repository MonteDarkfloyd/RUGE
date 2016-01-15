#include "sessionsaver.h"

#include "trafficwindow.h"
#include "ui_trafficwindow.h"

#include "session.h"
#include "newsessiondialog.h"


#include "createsession.h"

#include "sessionloader.h"
#include "sessionsaver.h"
#include <QtXml>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

SessionSaver::SessionSaver(Session *session, QString filename)
{
     session_= session;
     filename_ = filename;
}

void SessionSaver::Save_Session()
{

    QFile xmlFile2(filename_);

    QFile xmlFile("empty_UDP_session template.xml");
    xmlFile.open(QIODevice::ReadWrite);
    QByteArray xmlData(xmlFile.readAll());

    QDomDocument doc;
    doc.setContent(xmlData);

    QDomElement root = doc.documentElement();
    QDomElement sess_var_node = root.firstChildElement("RUGE_SESSION_VARIABLES");

    if(session_->srcMAC!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_VARIABLE ");
        ui.setAttribute("MAX", "");
        ui.setAttribute("MIN", "0");
        ui.setAttribute("TYPE","Binary");
        ui.setAttribute("LOOP_INCREMENT","0");
        ui.setAttribute("DEFAULT",session_->srcMAC);
        ui.setAttribute("INCREMENT","0");
        ui.setAttribute("SIZE",6);
        ui.setAttribute("VARIABLE","MAC_SRC");
        sess_var_node.appendChild(ui);
    }

    if(session_->dstMAC!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_VARIABLE ");
        ui.setAttribute("MAX", "");
        ui.setAttribute("MIN", "0");
        ui.setAttribute("TYPE","Binary");
        ui.setAttribute("LOOP_INCREMENT","0");
        ui.setAttribute("DEFAULT",session_->dstMAC);
        ui.setAttribute("INCREMENT","0");
        ui.setAttribute("SIZE",6);
        ui.setAttribute("VARIABLE","MAC_DST");
        sess_var_node.appendChild(ui);
    }

    if(session_->srcIP!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_VARIABLE ");
        ui.setAttribute("MAX", "");
        ui.setAttribute("MIN", "0");
        ui.setAttribute("TYPE","Binary");
        ui.setAttribute("LOOP_INCREMENT","0");
        ui.setAttribute("DEFAULT",session_->srcIP);
        ui.setAttribute("INCREMENT","0");
        ui.setAttribute("SIZE",4);
        ui.setAttribute("VARIABLE","IP_SRC");
        sess_var_node.appendChild(ui);
    }

    if(session_->dstIP!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_VARIABLE ");
        ui.setAttribute("MAX", "");
        ui.setAttribute("MIN", "0");
        ui.setAttribute("TYPE","Binary");
        ui.setAttribute("LOOP_INCREMENT","0");
        ui.setAttribute("DEFAULT",session_->dstIP);
        ui.setAttribute("INCREMENT","0");
        ui.setAttribute("SIZE",4);
        ui.setAttribute("VARIABLE","IP_DST");
        sess_var_node.appendChild(ui);
    }


    QDomElement sess_cont_msgs = root.firstChildElement("RUGE_SESSION_CONTROL_MESSAGES");


    QDomElement sess_cont_mes = sess_cont_msgs.firstChildElement("RUGE_SESSION_CONTROL_MESSAGE");
   // sess_cont_mes.setAttribute("NAME",session_->sessName);

    QDomElement sess_cont_mesg_head_stk = sess_cont_mes.firstChildElement("RUGE_SESSION_CONTROL_MESSAGE_HEADERS_STACK");
    sess_cont_mesg_head_stk.setAttribute("Protocol_1",session_->ipVersion);
    sess_cont_mesg_head_stk.setAttribute("Protocol_2",session_->protocol);

    if(session_->srcMAC!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES");
        ui.setAttribute("OFFSET", "6");
        ui.setAttribute("VARIABLE", "MAC_SRC");
        sess_cont_mes.appendChild(ui);
    }

    if(session_->dstMAC!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES");
        ui.setAttribute("OFFSET", "0");
        ui.setAttribute("VARIABLE", "MAC_DST");
        sess_cont_mes.appendChild(ui);
    }

    if(session_->srcIP!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES");
        ui.setAttribute("OFFSET", "26");
        ui.setAttribute("VARIABLE", "IP_SRC");
        sess_cont_mes.appendChild(ui);
    }

    if(session_->dstIP!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES");
        ui.setAttribute("OFFSET", "30");
        ui.setAttribute("VARIABLE", "IP_DST");
        sess_cont_mes.appendChild(ui);
    }

    QDomElement ref_data = sess_cont_mes.firstChildElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_REFERENCE_DATA");
    QString s=ref_data.attribute("VALUE");
    QString value = s + QString::fromLatin1(session_->payload.toLatin1().toHex());
    ref_data.setAttribute("LENGTH",value.length());
    ref_data.setAttribute("VALUE",  value);



   /* QDomElement elt = sess_cont_mes.firstChildElement("RUGE_SESSION_DECODE_INFO_LEVEL1");
    for (; !elt.isNull(); elt = elt.nextSiblingElement("RUGE_SESSION_DECODE_INFO_LEVEL1")) {
        if(elt.attribute("FIELD_NAME")=="udp.checksum"){

        }
    }*/

    QDomElement elt = sess_cont_mes.firstChildElement("RUGE_SESSION_DECODE_INFO_LEVEL1");
       for (; !elt.isNull(); elt = elt.nextSiblingElement("RUGE_SESSION_DECODE_INFO_LEVEL1")) {
           if(elt.attribute("FIELD_NAME")=="ip.ttl")
                elt.setAttribute("FIELD_NAME","IP_TTL");
           else if(elt.attribute("FIELD_NAME")=="udp.srcport")
                elt.setAttribute("FIELD_NAME","UDP_SRC_PORT");
           else if(elt.attribute("FIELD_NAME")=="udp.dstport")
                elt.setAttribute("FIELD_NAME","UDP_DST_PORT");
           else if(elt.attribute("FIELD_NAME")=="udp.length")
                elt.setAttribute("FIELD_NAME","UDP_LENGTH");
       }

       QDomElement sess_interfaces = root.firstChildElement("RUGE_SESSION_INTERFACES");
          QDomElement sess_interface = sess_interfaces.firstChildElement("RUGE_SESSION_INTERFACE");
          for (; !sess_interface.isNull(); sess_interface = sess_interface.nextSiblingElement("RUGE_SESSION_INTERFACE")) {
              if(sess_interface.attribute("NAME")=="Procedure 1"){
                 QDomElement sess_flow = sess_interface.firstChildElement("RUGE_SESSION_FLOW");
                 QDomElement sess_flow_item = sess_flow.firstChildElement("RUGE_SESSION_FLOW_ITEM");
                 for (; !sess_flow_item.isNull(); sess_flow_item = sess_flow_item.nextSiblingElement("RUGE_SESSION_FLOW_ITEM")) {
                     if(sess_flow_item.attribute("TYPE")=="MESSAGE")
                         sess_flow_item.attribute("ITEM")=session_->sessName;
                 }
              }
          }

    // save changes
     xmlFile2.open(QIODevice::ReadWrite);
    xmlFile2.resize(0);
    QTextStream stream;
    stream.setDevice(&xmlFile2);
    doc.save(stream, 4);

    xmlFile2.close();
}

