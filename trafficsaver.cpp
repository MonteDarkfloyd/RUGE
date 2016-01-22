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

void TrafficSaver::saveTraffic(bool askOverwrite){
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
        ui.setAttribute("SESSION_START_OFFSET_IN_US", sessionList_.at(i)->getOffset() );
        ui.setAttribute("CHECKSTATE", "Checked" );
        ui.setAttribute("SESSION_LOOP_OVER_COUNT", sessionList_.at(i)->getLoopover() );
        ui.setAttribute("SESSION_COUNT", sessionList_.at(i)->getMultiply() );
        ui.setAttribute("SESSION_LOOP_OVER_TIME_SPAN_IN_US", sessionList_.at(i)->getLoopoverTimespan() );
        ui.setAttribute("SESSION_NAME", sessionList_.at(i)->getName() );
        ui.setAttribute("SESSION_RAMPUP_INTERVAL_IN_US", sessionList_.at(i)->getRampup() );
        sess_var_node.appendChild(ui);

        // Add .xml to name of necessary
        QString rawName = sessionList_.at(i)->getName();
        addXML(rawName);

        sessionList_.at(i)->setName(rawName);

        if(xmlFile.exists() && askOverwrite){
            // Create a messagebox that asks overwriting
            QMessageBox::StandardButton overw;
            QString overwriteText = "File " + sessionList_.at(i)->getName() + " already exists.\nOverwrite?";
            overw = QMessageBox::warning(0, "Overwrite?", overwriteText,
                                          QMessageBox::Yes|QMessageBox::No);
            if (overw == QMessageBox::Yes) {
                SessionSaver saver(sessionList_.at(i),sessionList_.at(i)->getName());
                saver.Save_Session();
            }
            else {
              qDebug() << "Skipped a file";
            }
          }
        else{
            SessionSaver saver(sessionList_.at(i),sessionList_.at(i)->getName());
            saver.Save_Session();
        }
    }

    // Save to file
   xmlFile2.open(QIODevice::ReadWrite);
   xmlFile2.resize(0);
   QTextStream stream;
   stream.setDevice(&xmlFile2);
   doc.save(stream, 4);

   xmlFile2.close();

}

void TrafficSaver::addXML(QString &name){
    if(name.endsWith(".xml",Qt::CaseSensitive)){
        return;
    }
    else{
        name.append(".xml");
    }
return;
}
