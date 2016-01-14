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

SessionSaver::SessionSaver(QList<Session *> sessionList,int lastRow,QString filename)
{
     this->sessionList=sessionList;
     this->lastRow=lastRow;
     this->filename=filename;
}

void SessionSaver::Save_Session()
{

    QFile xmlFile2(filename);

    QFile xmlFile("empty_UDP_session template.xml");
    xmlFile.open(QIODevice::ReadWrite);
    QByteArray xmlData(xmlFile.readAll());

    QDomDocument doc;
    doc.setContent(xmlData);

    QDomElement root = doc.documentElement();
    QDomElement sess_var_node = root.firstChildElement("RUGE_SESSION_VARIABLES");

    if(sessionList.at(lastRow)->srcMAC!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_VARIABLE ");
        ui.setAttribute("MAX", "");
        ui.setAttribute("MIN", "0");
        ui.setAttribute("TYPE","Binary");
        ui.setAttribute("LOOP_INCREMENT","0");
        ui.setAttribute("DEFAULT",sessionList.at(lastRow)->srcMAC);
        ui.setAttribute("INCREMENT","0");
        ui.setAttribute("SIZE",6);
        ui.setAttribute("VARIABLE","MAC_SRC");
        sess_var_node.appendChild(ui);
    }

    if(sessionList.at(lastRow)->dstMAC!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_VARIABLE ");
        ui.setAttribute("MAX", "");
        ui.setAttribute("MIN", "0");
        ui.setAttribute("TYPE","Binary");
        ui.setAttribute("LOOP_INCREMENT","0");
        ui.setAttribute("DEFAULT",sessionList.at(lastRow)->dstMAC);
        ui.setAttribute("INCREMENT","0");
        ui.setAttribute("SIZE",6);
        ui.setAttribute("VARIABLE","MAC_DST");
        sess_var_node.appendChild(ui);
    }

    if(sessionList.at(lastRow)->srcIP!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_VARIABLE ");
        ui.setAttribute("MAX", "");
        ui.setAttribute("MIN", "0");
        ui.setAttribute("TYPE","Binary");
        ui.setAttribute("LOOP_INCREMENT","0");
        ui.setAttribute("DEFAULT",sessionList.at(lastRow)->srcIP);
        ui.setAttribute("INCREMENT","0");
        ui.setAttribute("SIZE",4);
        ui.setAttribute("VARIABLE","IP_SRC");
        sess_var_node.appendChild(ui);
    }

    if(sessionList.at(lastRow)->dstIP!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_VARIABLE ");
        ui.setAttribute("MAX", "");
        ui.setAttribute("MIN", "0");
        ui.setAttribute("TYPE","Binary");
        ui.setAttribute("LOOP_INCREMENT","0");
        ui.setAttribute("DEFAULT",sessionList.at(lastRow)->dstIP);
        ui.setAttribute("INCREMENT","0");
        ui.setAttribute("SIZE",4);
        ui.setAttribute("VARIABLE","IP_DST");
        sess_var_node.appendChild(ui);
    }


    QDomElement sess_cont_msgs = root.firstChildElement("RUGE_SESSION_CONTROL_MESSAGES");


    QDomElement sess_cont_mes = sess_cont_msgs.firstChildElement("RUGE_SESSION_CONTROL_MESSAGE");
    sess_cont_mes.setAttribute("NAME",sessionList.at(lastRow)->sessName);

    QDomElement sess_cont_mesg_head_stk = sess_cont_mes.firstChildElement("RUGE_SESSION_CONTROL_MESSAGE_HEADERS_STACK");
    sess_cont_mesg_head_stk.setAttribute("Protocol_1",sessionList.at(lastRow)->ipVersion);
    sess_cont_mesg_head_stk.setAttribute("Protocol_2",sessionList.at(lastRow)->protocol);

    if(sessionList.at(lastRow)->srcMAC!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES");
        ui.setAttribute("OFFSET", "6");
        ui.setAttribute("VARIABLE", "MAC_SRC");
        sess_cont_mes.appendChild(ui);
    }

    if(sessionList.at(lastRow)->dstMAC!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES");
        ui.setAttribute("OFFSET", "0");
        ui.setAttribute("VARIABLE", "MAC_DST");
        sess_cont_mes.appendChild(ui);
    }

    if(sessionList.at(lastRow)->srcIP!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES");
        ui.setAttribute("OFFSET", "26");
        ui.setAttribute("VARIABLE", "IP_SRC");
        sess_cont_mes.appendChild(ui);
    }

    if(sessionList.at(lastRow)->dstIP!="")
    {
        QDomElement ui = doc.createElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES");
        ui.setAttribute("OFFSET", "30");
        ui.setAttribute("VARIABLE", "IP_DST");
        sess_cont_mes.appendChild(ui);
    }

    QDomElement ref_data = sess_cont_mes.firstChildElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_REFERENCE_DATA");
    QString s=ref_data.attribute("VALUE");
    QString value = s + QString::fromLatin1(sessionList.at(lastRow)->payload.toLatin1().toHex());
    ref_data.setAttribute("VALUE",  value);
    ref_data.setAttribute("LENGTH",value.length());


   /* QDomElement elt = sess_cont_mes.firstChildElement("RUGE_SESSION_DECODE_INFO_LEVEL1");
    for (; !elt.isNull(); elt = elt.nextSiblingElement("RUGE_SESSION_DECODE_INFO_LEVEL1")) {
        if(elt.attribute("FIELD_NAME")=="udp.checksum"){

        }
    }*/


    // save changes
     xmlFile2.open(QIODevice::ReadWrite);
    xmlFile2.resize(0);
    QTextStream stream;
    stream.setDevice(&xmlFile2);
    doc.save(stream, 4);

    xmlFile2.close();
}

