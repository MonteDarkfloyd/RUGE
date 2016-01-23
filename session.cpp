#include "session.h"

Session::Session (){
    name_ = "";
    payload_ = "";
    protocol_ = "";
    ipVersion_ = "";
    multiply_ = "10";
    rampup_ = "100";
    offset_ = "100";
    loopover_ = "1";
    loopovertimespan_ = "0";
}

Session::~Session(){
}

QString Session::getName(){
    return name_;
}

void Session::setName(QString sessionName){
    name_ = sessionName;
}

// Adds .xml to the session name if it doesn't have one.
// If name is empty. does nothing.
void Session::addXMLextension(){
    if(name_.endsWith(".xml",Qt::CaseSensitive) || name_ == ""){
        return;
    }
    else{
        name_.append(".xml");
    }
return;
}

QList<rugeVariable>* Session::getVariables(){
    return &data_;
}

// Adds variable to the Session. Doesn't check if variable is
// valid. If variable with the same name exist, then the old one
// is deleted and the new one added.
void Session::addVariable(rugeVariable added){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == added.name){
            data_.removeAt(i);
        }
    }
    data_.append(added);
}

// Deletes variable with name varName.
// If it does not exist this does nothing.
void Session::removeVariable(QString varName){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == varName){
            data_.removeAt(i);
        }
    }
}

// Returns the rugeVariable with the name of varName.
// If doesn't exist, return empty rugeVariable.
rugeVariable Session::getVariable(QString varName){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == varName){
            return data_.at(i);
        }
    }
    rugeVariable empty = {"","","","","",0,0,0,0};
    return empty;
}


// Returns source IP data. If there is not one
// return empty data.
rugeVariable Session::getSrcIP(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "IP_SRC"){
            return data_.at(i);
        }
    }
    rugeVariable empty = {"","","","","",0,0,0,0};
    return empty;
}

// Returns destination IP data. If there is not one
// return empty data.
rugeVariable Session::getDstIP(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "IP_DST"){
            return data_.at(i);
        }
    }
    rugeVariable empty = {"","","","","",0,0,0,0};
    return empty;
}

// Get the MAC values as string
QString Session::getSrcMAC(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "MAC_SRC"){
            return data_.at(i).value;
        }
    }
    return "";
}

QString Session::getDstMAC(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "MAC_DST"){
            return data_.at(i).value;
        }
    }
    return "";
}

// Get port values
QString Session::getSrcPort(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "UDP_SRC_PORT"){
            return data_.at(i).value;
        }
    }
    return "";
}

QString Session::getDstPort(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "UDP_DST_PORT"){
            return data_.at(i).value;
        }
    }
    return "";
}

// Get and set payload
QString Session::getPayload(){
    return payload_;
}

void Session::setPayload(QString payload){
    payload_ = payload;
}

// Get and set protocol
QString Session::getProtocol(){
    return protocol_;
}


void Session::setProtocol(QString protocol){
    protocol_ = protocol;
}

// Get and set ipVersion
QString Session::getipVersion(){
    return ipVersion_;
}

void Session::setipVersion(QString ipVersion){
    ipVersion_ = ipVersion;
}


// Get the values that are saved in
// the traffic profile
QString Session::getMultiply(){
    return multiply_;
}

QString Session::getRampup(){
    return rampup_;
}

QString Session::getOffset(){
    return offset_;
}

QString Session::getLoopover(){
    return loopover_;
}

QString Session::getLoopoverTimespan(){
    return loopovertimespan_;
}

// Used to set values that are saved in traffic profile.
void Session::setMultiply(QString newMultiply){
    multiply_ = newMultiply;
}

void Session::setRampup(QString newRampup){
    rampup_ = newRampup;
}

void Session::setOffset(QString newOffset){
    offset_ = newOffset;
}

void Session::setLoopover(QString newLoopover){
    loopover_ = newLoopover;
}

void Session::setLoopoverTimespan(QString newLoopoverTimespan){
    loopovertimespan_ = newLoopoverTimespan;
}
