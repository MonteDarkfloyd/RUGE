#ifndef TRAFFICSAVER_H
#define TRAFFICSAVER_H
#include "session.h"


class TrafficSaver
{
public:

    // Constructor. Parameters are the trafficfile string that tells
    // the filename of the traffic profile xml.
    // sessionList has all the sessions that are in this traffic profile.
    TrafficSaver(QString trafficfile, QList<Session *> sessionList);

    // Saves the traffic profile and all the sessions in it.
    // Parameter: askOverwrite, if true, for every session the
    // overwriting is asked from user. If false sessions are saved
    // without asking.
    void saveTraffic(bool askOverwrite = true);

private:
    QString filename_;
    QList<Session*> sessionList_;
};

#endif // TRAFFICSAVER_H
