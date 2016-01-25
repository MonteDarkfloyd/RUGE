#include "trafficsaver.h"
#include "sessionsaver.h"
#include <QFile>
#include <QtXml>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>


// Parameters are the trafficfile string that tells
// the filename of the traffic profile xml.
// sessionList has all the sessions that are in this traffic profile.
TrafficSaver::TrafficSaver(QString trafficfile, QList<Session*> sessionList)
{
    filename_ = trafficfile;
    sessionList_ = sessionList;
}


// Saves the traffic profile and all the sessions in it.
// Parameter: askOverwrite, if true, for every session the
// overwriting is asked from user. If false sessions are saved
// without asking.
void TrafficSaver::saveTraffic(bool askOverwrite){
    QFile xmlFile2(filename_);

    // We use template for saving.
    QFile xmlFile("UDP_traffic_template.xml");
    xmlFile.open(QIODevice::ReadWrite);

    // Make dom document from the template.
    QByteArray xmlData(xmlFile.readAll());
    QDomDocument doc;
    doc.setContent(xmlData);

    QDomElement root = doc.documentElement();
    QDomElement sess_var_node = root.firstChildElement("SESSIONS");

    // Save sessions one by one
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
        sessionList_.at(i)->addXMLextension();

        QFile xmlFile2("session_profiles/" + sessionList_.at(i)->getName());

        // Check if we need to ask about overwriting
        if(xmlFile2.exists() && askOverwrite){

            // Create a messagebox that asks overwriting
            QMessageBox::StandardButton overw;
            QString overwriteText = "File " + sessionList_.at(i)->getName() + " already exists.\nOverwrite?";
            overw = QMessageBox::warning(0, "Overwrite?", overwriteText,
                                          QMessageBox::Yes|QMessageBox::No);
            // User wants to overwrite the session
            if (overw == QMessageBox::Yes) {
                SessionSaver saver(sessionList_.at(i),"session_profiles/" + sessionList_.at(i)->getName());
                saver.Save_Session();
            }
        }

        // File does not exist or we can overwrite all.
        else{
            SessionSaver saver(sessionList_.at(i),"session_profiles/" + sessionList_.at(i)->getName());
            saver.Save_Session();
        }
    }

   // Save to wanted file
   xmlFile2.open(QIODevice::ReadWrite);
   xmlFile2.resize(0);
   QTextStream stream;
   stream.setDevice(&xmlFile2);
   doc.save(stream, 4);

   xmlFile2.close();
   xmlFile.close();

}
