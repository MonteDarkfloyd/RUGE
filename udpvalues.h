#ifndef UDPVALUES_H
#define UDPVALUES_H
#include <QString>


class UDPvalues
{
public:


    UDPvalues ();
    UDPvalues(QString _srcPort, QString _dstPort ,
              QString _length);

    QString srcPort;
    QString dstPort;
    QString length;

};

#endif // UDPVALUES_H
