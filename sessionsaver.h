#ifndef SESSIONSAVER_H
#define SESSIONSAVER_H

#include <session.h>

class SessionSaver
{
public:

    // Constructor. Parameters: session that is saved and
    // filename where it is saved.
    SessionSaver(Session* session,QString filename);

    // Save the session to the file.
    void Save_Session();

private:
    Session* session_;
    QString filename_;
};


#endif // SESSIONSAVER_H
