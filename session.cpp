#include "session.h"

Session::Session(QString name, QString sourceIP, QString destinationIP,
                 QString sourceMAC,QString destinationMAC, QString _payload,
                 QString _protocol, QString _ipversion,
                 int _multiply, int _rampup,
                 int _offset, int _loopover,
                 int _loopovertimespan )
{
    this->sessName = name;
    this->srcIP = sourceIP;
    this->dstIP = destinationIP;
    this->srcMAC = sourceMAC;
    this->dstMAC = destinationMAC;
    this->payload = _payload;
    this->protocol = _protocol;
    this->ipVersion = _ipversion;

    this->multiply = _multiply;
    this->rampup = _rampup;
    this->offset = _offset;
    this->loopover = _loopover;
    this->loopovertimespan = _loopovertimespan;

}

