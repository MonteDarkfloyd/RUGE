#ifndef TCPVALUES_H
#define TCPVALUES_H

#include <QString>


class TCPvalues
{
public:
    TCPvalues(QString _srcPort,
              QString _dstPort,
              QString _seqNo,
              QString _window,
              QString _ackNo,
              bool urg ,
              bool ack ,
              bool psh ,
              bool rst ,
              bool syn ,
              bool fin
             );

    QString srcPort;
    QString dstPort;
    QString seqNo;
    QString window;
    QString ackNo;
    bool urg;
    bool ack;
    bool psh;
    bool rst;
    bool syn;
    bool fin;

};

#endif // TCPVALUES_H
