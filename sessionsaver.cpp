#include "sessionsaver.h"
#include "session.h"

#include <QtXml>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>

SessionSaver::SessionSaver(Session *session, QString filename)
{
     session_= session;
     filename_ = filename;
}

void SessionSaver::Save_Session()
{

    QFile xmlFile2(filename_);

    // We use template file to create the final xml.
    QFile xmlFile("UDP_session_template.xml");
    xmlFile.open(QIODevice::ReadWrite);

    // Make dom document from the template
    QByteArray xmlData(xmlFile.readAll());
    QDomDocument doc;
    doc.setContent(xmlData);

    QDomElement root = doc.documentElement();
    QDomElement sess_var_node = root.firstChildElement("RUGE_SESSION_VARIABLES");
    QList<rugeVariable>* sessionVariables = session_->getVariables();

    // Add the session variables to the RUGE_SESSION_VARIABLES
    for(int i = 0; i < sessionVariables->size(); i++){
       QDomElement ui = doc.createElement("RUGE_SESSION_VARIABLE ");
       ui.setAttribute("MAX", sessionVariables->at(i).max);
       ui.setAttribute("MIN", sessionVariables->at(i).min);
       ui.setAttribute("TYPE",sessionVariables->at(i).type);
       ui.setAttribute("LOOP_INCREMENT",sessionVariables->at(i).loopIncrement);
       ui.setAttribute("DEFAULT",sessionVariables->at(i).value);
       ui.setAttribute("INCREMENT",sessionVariables->at(i).increment);
       ui.setAttribute("SIZE",sessionVariables->at(i).size);
       ui.setAttribute("VARIABLE",sessionVariables->at(i).name);
       sess_var_node.appendChild(ui);
    }

    QDomElement sess_cont_msgs = root.firstChildElement("RUGE_SESSION_CONTROL_MESSAGES");
    QDomElement sess_cont_mes = sess_cont_msgs.firstChildElement("RUGE_SESSION_CONTROL_MESSAGE");

    // Save the ip version and protocol
    QDomElement sess_cont_mesg_head_stk = sess_cont_mes.firstChildElement("RUGE_SESSION_CONTROL_MESSAGE_HEADERS_STACK");
    sess_cont_mesg_head_stk.setAttribute("Protocol_1",session_->getipVersion());
    sess_cont_mesg_head_stk.setAttribute("Protocol_2",session_->getProtocol());

    // Save the RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES
    for(int i = 0; i < sessionVariables->size(); i++){
        QDomElement ui = doc.createElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_VARIABLES");
        ui.setAttribute("OFFSET", sessionVariables->at(i).offset);
        ui.setAttribute("VARIABLE", sessionVariables->at(i).name);
        sess_cont_mes.appendChild(ui);
    }


    // Save the payload.
    QDomElement ref_data = sess_cont_mes.firstChildElement("RUGE_SESSION_CONTROL_MESSAGE_HEADER_REFERENCE_DATA");
    QString s=ref_data.attribute("VALUE");
    QString value = s + QString::fromLatin1(session_->getPayload().toLatin1().toHex());
    ref_data.setAttribute("LENGTH",value.length());
    ref_data.setAttribute("VALUE",  value);

    QDomElement sess_interfaces = root.firstChildElement("RUGE_SESSION_INTERFACES");
    QDomElement sess_interface = sess_interfaces.firstChildElement("RUGE_SESSION_INTERFACE");

    // Save this file to the filename.
    xmlFile2.open(QIODevice::ReadWrite);
    xmlFile2.resize(0);
    QTextStream stream;
    stream.setDevice(&xmlFile2);
    doc.save(stream, 4);
    xmlFile.close();
    xmlFile2.close();
}

