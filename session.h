#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "tcpvalues.h"
#include "udpvalues.h"
#include "variabledata.h"


class Session
{

public:
    Session ();
    ~Session();


    // Get and set session name.
    QString getName();
    void setName(QString sessionName);
    void addXMLextension();

    QList<rugeVariable> *getVariables();
    void addVariable(rugeVariable added);
    void removeVariable(QString varName);
    rugeVariable getVariable(QString varName);

    // Get the ip value. Used for getting the
    // ip address and the max ip address at the same time.
    rugeVariable getSrcIP();
    rugeVariable getDstIP();

    // Get the MAC value as string
    QString getSrcMAC();
    QString getDstMAC();

    // Get port values
    QString getSrcPort();
    QString getDstPort();

    // Set and get Payload
    QString getPayload();
    void setPayload(QString payload);

    // Set and get protocol
    QString getProtocol();
    void setProtocol(QString protocol);

    // Set and get ipVersion
    QString getipVersion();
    void setipVersion(QString ipVersion);

    // Get the values that are saved in
    // the traffic profile.
    QString getMultiply();
    QString getRampup();
    QString getOffset();
    QString getLoopover();
    QString getLoopoverTimespan();

    // Set the values that are saved in the
    // traffic profile.
    void setMultiply(QString newMultiply);
    void setRampup(QString newRampup);
    void setOffset(QString newOffset);
    void setLoopover(QString newLoopover);
    void setLoopoverTimespan(QString newLoopoverTimespan);

signals:

public slots:
private:

    QString name_;
    QString payload_;
    QString protocol_;
    QString ipVersion_;

    QString multiply_; // Max is 1000000
    QString rampup_; // Maximum is 1000000000
    QString offset_; // Maximum is 1000000000
    QString loopover_; // Maximum is 1000000
    QString loopovertimespan_; // Max is 10000000000
    QList<rugeVariable> data_;
};

#endif // SESSION_H
