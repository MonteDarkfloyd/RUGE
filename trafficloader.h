#ifndef TRAFFICLOADER_H
#define TRAFFICLOADER_H
#include "session.h"
#include <QXmlStreamReader>

class TrafficLoader
{
public:
    TrafficLoader(QString trafficfile);
    ~TrafficLoader();

    // Check the traffic profile file and the sessions in it.
    // Return value boolean: false if failure and true if success.
    // Parameter: error, the error message text is saved here.
    // Use before loadTraffic function.
    bool checkTraffic(QString &error);

    // Returns the sessions that were saved in the traffic profile.
    QList<Session*> loadTraffic();

signals:

public slots:
private:

    // Adds the values that are found in the traffic profile xml
    // to the session class.
    // Return value boolean false if failure and true if success.
    // Parameters: session, the session where the values are saved.
    // error, error message is saved here.
    bool addTrafficvalues(Session *session, QString &error);

    QString filename_;
    bool loaded_;
    QXmlStreamReader xml_;
    QList<Session *> loadedList_;
    QString error_;
};

#endif // TRAFFICLOADER_H
