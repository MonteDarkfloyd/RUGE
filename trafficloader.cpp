#include "trafficloader.h"
#include "sessionloader.h"
#include <QFile>

TrafficLoader::TrafficLoader(QString trafficfile):filename_(trafficfile), loaded_(false), error_("")
{

}

TrafficLoader::~TrafficLoader(){

    // If the session in this traffic file were never loaded
    // then delete all of them.
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

        // Checking sessions starts here
        xml_.readNextStartElement();

        if(xml_.name() != "SESSIONS"){
            error = "XML could not be parsed!";
            return false;
        }
        xml_.readNextStartElement();

        while(xml_.name() == "SESSION" && !xml_.atEnd()){
            if(!xml_.attributes().hasAttribute("SESSION_NAME")){
                error_ = "XML could not be parsed!";
                return false;
            }
            else{

                // Try to load the session file from session_profiles folder.
                QString sessionName = xml_.attributes().value("SESSION_NAME").toString();
                SessionLoader loader("session_profiles/" + sessionName);

                if(!loader.checkSession(error)){
                    error = "Session included in this traffic profile couldn't not be parsed.";
                    return false;
                }

                else{
                    Session* sess = loader.loadSession();

                    if(!addTrafficvalues(sess,error)){
                        return false;
                    }
                    loadedList_.push_back(sess);
                }
            }

            xml_.readNextStartElement();
            if(xml_.isEndElement()){
                xml_.readNextStartElement();
            }
         }

    }
    return true;
}

// Return loaded sessions and set the loaded_ to true so
// that when this is destructed the sessions aren't deleted.
QList<Session*> TrafficLoader::loadTraffic(){
    loaded_ = true;
    return loadedList_;
}

// Adds the values that are found in the traffic profile xml
// to the session class.
// Return value boolean false if failure and true if success.
// Parameters: session, the session where the values are saved.
// error, error message is saved here.
bool TrafficLoader::addTrafficvalues(Session* session, QString &error){

    // First check that the attributes are found.
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
         // Set the values to the session.
         session->setOffset(xml_.attributes().value("SESSION_START_OFFSET_IN_US").toString());
         session->setLoopover(xml_.attributes().value("SESSION_LOOP_OVER_COUNT").toString());
         session->setLoopoverTimespan(xml_.attributes().value("SESSION_LOOP_OVER_TIME_SPAN_IN_US").toString());
         session->setMultiply(xml_.attributes().value("SESSION_COUNT").toString());
         session->setRampup(xml_.attributes().value("SESSION_RAMPUP_INTERVAL_IN_US").toString());
         return true;
     }
     return false;
 }
