#ifndef SESSIONSAVER_H
#define SESSIONSAVER_H

#include <session.h>

class SessionSaver
{
public:

    SessionSaver(QList<Session *> sessionList,int lastRow,QString filename);
    void Save_Session();
private:
    QList<Session *> sessionList;
    int lastRow;
    QString filename;
};


#endif // SESSIONSAVER_H
