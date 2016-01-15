#include "trafficloader.h"
#include "sessionloader.h"
#include <QFile>
#include <QDebug>

TrafficLoader::TrafficLoader(QString trafficfile):filename_(trafficfile), loaded_(false), error_("")
{

}

TrafficLoader::~TrafficLoader(){

    if(!loaded_){
        for(int i = 0; i < loadedList_.size(); ++i){
            delete loadedList_.at(i);
        }
    }
}


// Load traffic profile file. Return value false if
// loading fails. QString error has error message text.
// Use before loadTraffic function.
bool TrafficLoader::checkTraffic(QString &error){

    QFile xmlFile(filename_);
    if(!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        error = "Failed to open the file for reading.";
        return false;
    }

    xml_.setDevice(&xmlFile);

    // Read start of the document.
    xml_.readNext();

    // Read the DTD data from the xml
    xml_.readNext();

    // The file is empty or very short?
    if (xml_.hasError()){
        error = "XML could not be parsed!";
        return false;
    }
    // Check if DTD = RUGESessionData
    if (xml_.dtdName() != "RUGETrafficProfile"){
        error = "XML is not the right type.";
        return false;
    }

    if (xml_.readNext() && xml_.name() == "TRAFFIC_PROFILE"){
        qDebug() << xml_.name();

        // Checking sessions starts here
        xml_.readNextStartElement();

        if(xml_.name() != "SESSIONS"){
            error = "XML could not be parsed!";
            return false;
        }
        xml_.readNextStartElement();
        qDebug() << "Loop start " << xml_.name();

        while(xml_.name() == "SESSION" && !xml_.atEnd()){
            if(!xml_.attributes().hasAttribute("SESSION_NAME")){
                error_ = "XML could not be parsed!";
                return false;
            }
            else{
                QString sessionName = xml_.attributes().value("SESSION_NAME").toString();
                SessionLoader loader(sessionName);

                if(!loader.checkSession(error)){
                    error = "Session included in this traffic profile couldn't not be parsed.";
                    return false;
                }

                else{
                    qDebug() << "session found";
                    Session* sess = loader.loadSession();

                    if(!addTrafficvalues(sess,error)){
                        return false;
                    }
                    qDebug() << "session saved";
                    loadedList_.push_back(sess);
                }
            }

            xml_.readNextStartElement();
            if(xml_.isEndElement()){
                xml_.readNextStartElement();
            }

            qDebug() << xml_.name() << "  Loop1";
         }

    }
    return true;
}

QList<Session*> TrafficLoader::loadTraffic(){
    qDebug() << loadedList_.size();
    loaded_ = true;
    return loadedList_;
}

bool TrafficLoader::addTrafficvalues(Session* session, QString &error){

     if(!xml_.attributes().hasAttribute("SESSION_LOOP_OVER_COUNT")){
         error = "Session has no loop over count.";
         return false;
     }

     else if(!xml_.attributes().hasAttribute("SESSION_LOOP_OVER_TIME_SPAN_IN_US")){
         error = "Session has no loop over time span.";
         return false;
     }
     else if(!xml_.attributes().hasAttribute("SESSION_COUNT")){
         error = "Session has no multiply count.";
         return false;
     }
     else if(!xml_.attributes().hasAttribute("SESSION_RAMPUP_INTERVAL_IN_US")){
         error = "Session has no rampup.";
         return false;
     }
     else if(!xml_.attributes().hasAttribute("SESSION_START_OFFSET_IN_US")){
         error = "Session has no offset.";
         return false;
     }
     else{
         session->offset = xml_.attributes().value("SESSION_START_OFFSET_IN_US").toInt();
         session->loopover = xml_.attributes().value("SESSION_LOOP_OVER_COUNT").toInt();
         session->loopovertimespan = xml_.attributes().value("SESSION_LOOP_OVER_TIME_SPAN_IN_US").toInt();
         session->multiply = xml_.attributes().value("SESSION_COUNT").toInt();
         session->rampup = xml_.attributes().value("SESSION_RAMPUP_INTERVAL_IN_US").toInt();
         return true;
     }
     return false;
 }
