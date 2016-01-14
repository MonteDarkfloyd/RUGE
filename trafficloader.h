#ifndef TRAFFICLOADER_H
#define TRAFFICLOADER_H
#include "session.h"
#include <QXmlStreamReader>

class TrafficLoader
{
public:
    TrafficLoader(QString trafficfile);
    ~TrafficLoader();

    // Load traffic profile file.
    bool checkTraffic(QString &error);

    // Returns the sessions that were saved in the traffic profile.
    QList<Session*> loadTraffic();

signals:

public slots:
private:

    bool addTrafficvalues(Session *session, QString &error);

    QString filename_;
    bool loaded_;
    QXmlStreamReader xml_;
    QList<Session *> loadedList_;
    QString error_;
};

#endif // TRAFFICLOADER_H
