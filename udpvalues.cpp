#include "udpvalues.h"

UDPvalues::UDPvalues(){
    srcPort = "";
    dstPort = "";
    length = "";
}

UDPvalues::UDPvalues(QString _srcPort, QString _dstPort, QString _length )
{
    this->srcPort = _srcPort;
    this->dstPort = _dstPort;
    this->length = _length;

}

