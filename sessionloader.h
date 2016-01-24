#ifndef SESSIONLOADER_H
#define SESSIONLOADER_H
#include <QObject>
#include <QXmlStreamReader>
#include "session.h"


class SessionLoader
{
public:

    // Give the session's file name to the
    // constructor.
    SessionLoader(QString sessionfile);
    ~SessionLoader();

    // Checks if session is valid.
    // Return value boolean: false if failed
    // true otherwise.
    // Parameter: error, error text is saved here
    bool checkSession(QString &error);

    // After the session is loaded properly
    // this will return pointer to it.
    // Use this after checkSession has returned true.
    Session* loadSession();


private:
    QString filename_;
    Session* session_;
    QXmlStreamReader xml_;
    bool sessionloaded_;
    QString message_;


    // Read and check the variable part of xml.
    bool readVariable();

    // Read and check the states part of xml.
    bool checkStates();

    // Read and check the controlmessages part of xml.
    bool checkControlMessage();

    // Read and create the payload from xml.
    bool buildPayload();

    // Read and check the timeout part of xml.
    bool checkTimeout();

    // Read and check the sessionflow part of xml.
    bool checkSessionflow();
};

#endif // SESSIONLOADER_H
