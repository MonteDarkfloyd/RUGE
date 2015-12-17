#ifndef SESSIONLOADER_H
#define SESSIONLOADER_H
#include <QObject>
#include <QXmlStreamReader>
#include "session.h"


class SessionLoader
{
public:
    SessionLoader(QString sessionfile);
    ~SessionLoader();

    bool checkSession(QString &error);
    Session* loadSession();


private:
    QString filename_;
    Session* session_;
    QXmlStreamReader xml_;
    bool sessionloaded_;
    QString message_;


    bool readVariable();
    bool checkStates();
    bool checkControlMessage();
    bool buildPayload();
    bool checkTimeout();
    bool checkSessionflow();
};

#endif // SESSIONLOADER_H
