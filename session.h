#ifndef SESSION_H
#define SESSION_H

#include <QObject>

class Session
{
public:
    Session(QString name, QString sourceIP, QString destinationIP,
            QString sourceMAC, QString destinationMAC, QString _payload,
            QString _protocol, QString _ipversion,
            int multiply = 10, int rampup = 100,
            int offset = 100, int loopover = 1,
            int loopovertimespan = 0);

    QString sessName;
    QString srcIP;
    QString dstIP;
    QString srcMAC;
    QString dstMAC;
    QString payload;
    QString protocol;
    QString ipVersion;

    int multiply;
    int rampup;
    int offset;
    int loopover;
    int loopovertimespan;

signals:

public slots:
};

#endif // SESSION_H
