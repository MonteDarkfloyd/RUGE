#ifndef TRAFFICSAVER_H
#define TRAFFICSAVER_H
#include "session.h"


class TrafficSaver
{
public:
    TrafficSaver(QString trafficfile, QList<Session *> sessionList);
    void saveTraffic(bool askOverwrite = true);
private:
    QString filename_;
    QList<Session*> sessionList_;
};

#endif // TRAFFICSAVER_H
