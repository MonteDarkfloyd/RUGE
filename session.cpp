#include "session.h"

Session::Session (){
    name_ = "";
    payload_ = "";
    protocol_ = "";
    ipVersion_ = "";
    multiply_ = "1";
    rampup_ = "100";
    offset_ = "0";
    loopover_ = "1";
    loopovertimespan_ = "1000";
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
// If name is empty, does nothing.
void Session::addXMLextension(){
    if(name_.endsWith(".xml",Qt::CaseSensitive) || name_ == ""){
        return;
    }
    else{
        name_.append(".xml");
    }
return;
}

// Returns all rugeVariables that the session has.
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
// If variable doesn't exist, return empty rugeVariable.
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
// return empty rugeVariable.
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
// return empty rugeVariable.
rugeVariable Session::getDstIP(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "IP_DST"){
            return data_.at(i);
        }
    }
    rugeVariable empty = {"","","","","",0,0,0,0};
    return empty;
}

// Get the MAC source as string
QString Session::getSrcMAC(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "MAC_SRC"){
            return data_.at(i).value;
        }
    }
    return "";
}

// Get the MAC destination as string
QString Session::getDstMAC(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "MAC_DST"){
            return data_.at(i).value;
        }
    }
    return "";
}

// Get source port as string
QString Session::getSrcPort(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "UDP_SRC_PORT"){
            return data_.at(i).value;
        }
    }
    return "";
}

// Get destination port as string
QString Session::getDstPort(){
    for(int i = 0; i < data_.size(); i++){
        if(data_.at(i).name == "UDP_DST_PORT"){
            return data_.at(i).value;
        }
    }
    return "";
}

// Get payload
QString Session::getPayload(){
    return payload_;
}

// Set the payload text
void Session::setPayload(QString payload){
    payload_ = payload;
}

// Get protocol
QString Session::getProtocol(){
    return protocol_;
}

// Set protocol
void Session::setProtocol(QString protocol){
    protocol_ = protocol;
}

// Get ipVersion
QString Session::getipVersion(){
    return ipVersion_;
}

// Set ipVersion
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
