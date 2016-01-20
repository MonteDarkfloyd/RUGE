#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "tcpvalues.h"
#include "udpvalues.h"


class Session
{

public:
    Session ();
    ~Session();
    Session(QString name, QString sourceIP, QString destinationIP,
            QString sourceMAC, QString destinationMAC, QString _payload,
            QString _protocol, QString _ipversion, TCPvalues* _tcp, UDPvalues* _udp,
            QString _multiply, QString _rampup,
            QString _offset, QString _loopover,
            QString _loopovertimespan, QString _srcIPmax, QString _dstIPmax);

    QString sessName;
    QString srcIP;
    QString dstIP;
    QString srcMAC;
    QString dstMAC;
    QString payload;
    QString protocol;
    QString ipVersion;
    QString ttl;
    TCPvalues *tcp;
    UDPvalues *udp;

    QString srcIPmax;
    QString dstIPmax;

    QString multiply;
    QString rampup;
    QString offset;
    QString loopover;
    QString loopovertimespan;

signals:

public slots:
};

#endif // SESSION_H
