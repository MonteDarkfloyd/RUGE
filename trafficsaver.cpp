#include "trafficsaver.h"
#include "sessionsaver.h"
#include <QFile>
#include <QtXml>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

TrafficSaver::TrafficSaver(QString trafficfile, QList<Session*> sessionList)
{
    filename_ = trafficfile;
    sessionList_ = sessionList;
}

void TrafficSaver::saveTraffic(){
    QFile xmlFile2(filename_);

    QFile xmlFile("empty_UDP_traffic template.xml");
    xmlFile.open(QIODevice::ReadWrite);
    QByteArray xmlData(xmlFile.readAll());

    QDomDocument doc;
    doc.setContent(xmlData);

    QDomElement root = doc.documentElement();
    QDomElement sess_var_node = root.firstChildElement("SESSIONS");

    for(int i = 0; i < sessionList_.size(); i++){

        QDomElement ui = doc.createElement("SESSION");
        ui.setAttribute("SESSION_START_OFFSET_IN_US", sessionList_.at(i)->offset );
        ui.setAttribute("CHECKSTATE", "Checked" );
        ui.setAttribute("SESSION_LOOP_OVER_COUNT", sessionList_.at(i)->loopover );
        ui.setAttribute("SESSION_COUNT", sessionList_.at(i)->multiply );
        ui.setAttribute("SESSION_LOOP_OVER_TIME_SPAN_IN_US", sessionList_.at(i)->loopovertimespan );
        ui.setAttribute("SESSION_NAME", sessionList_.at(i)->sessName );
        ui.setAttribute("SESSION_RAMPUP_INTERVAL_IN_US", sessionList_.at(i)->rampup );
        sess_var_node.appendChild(ui);
        // TODO: add xml to name if it doesn't have one
        // Check if overwriting happens.
        SessionSaver saver(sessionList_.at(i),sessionList_.at(i)->sessName);
        saver.Save_Session();
    }

    // Save to file
   xmlFile2.open(QIODevice::ReadWrite);
   xmlFile2.resize(0);
   QTextStream stream;
   stream.setDevice(&xmlFile2);
   doc.save(stream, 4);

   xmlFile2.close();

}
