#include "session.h"

Session::Session (){
    sessName = "";
    srcIP = "";
    dstIP = "";
    srcMAC = "";
    payload = "";
    protocol = "";
    ipVersion = "";
    ttl = "";
    multiply = "10";
    rampup = "100";
    offset = "100";
    loopover = "1";
    loopovertimespan = "0";
    tcp = 0;
    dstIPmax = "";
    srcIPmax = "";
    udp = new UDPvalues();
}

Session::~Session(){
    delete udp;
}

Session::Session(QString name, QString sourceIP, QString destinationIP,
                 QString sourceMAC, QString destinationMAC, QString _payload,
                 QString _protocol, QString _ipversion, TCPvalues* _tcp, UDPvalues* _udp,
                 QString _multiply, QString _rampup,
                 QString _offset, QString _loopover,
                 QString _loopovertimespan , QString _srcIPmax, QString _dstIPmax)
{
    this->sessName = name;
    this->srcIP = sourceIP;
    this->dstIP = destinationIP;
    this->srcMAC = sourceMAC;
    this->dstMAC = destinationMAC;
    this->payload = _payload;
    this->protocol = _protocol;
    this->ipVersion = _ipversion;
    this->tcp = _tcp;
    this->udp = _udp;
    this->ttl = "";
    this->dstIPmax = _dstIPmax;
    this->srcIPmax = _srcIPmax;

    this->multiply = _multiply; // Max is 1000000
    this->rampup = _rampup; // Maximum is 1000000000
    this->offset = _offset; // Maximum is 1000000000
    this->loopover = _loopover; // Maximum is 1000000
    this->loopovertimespan = _loopovertimespan; // Max is 10000000000
}

