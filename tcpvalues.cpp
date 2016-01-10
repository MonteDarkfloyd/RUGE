#include "tcpvalues.h"

TCPvalues::    TCPvalues(QString _srcPort,
                         QString _dstPort,
                         QString _seqNo ,
                         QString _window,
                         QString _ackNo,
                         bool _urg,
                         bool _ack,
                         bool _psh,
                         bool _rst,
                         bool _syn,
                         bool _fin
                        )
{
    this->srcPort = _srcPort;
    this->dstPort = _dstPort;
    this->seqNo = _seqNo;
    this->window = _window;
    this->ackNo = _ackNo;
    this->urg = _urg;
    this->ack = _ack;
    this->psh = _psh;
    this->rst = _rst;
    this->syn = _syn;
    this->fin = _fin;

}

