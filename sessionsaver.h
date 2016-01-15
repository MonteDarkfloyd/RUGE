#ifndef SESSIONSAVER_H
#define SESSIONSAVER_H

#include <session.h>

class SessionSaver
{
public:

    SessionSaver(Session* session,QString filename);
    void Save_Session();
private:
    Session* session_;
    QString filename_;
};


#endif // SESSIONSAVER_H
