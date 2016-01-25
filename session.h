#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "variabledata.h"


class Session
{

public:
    Session ();
    ~Session();


    // Get session name.
    QString getName();

    // Set sessionName as sessions name
    void setName(QString sessionName);

    // Adds .xml to the session names end
    // if it doesn't end to .xml already.
    void addXMLextension();

    // Return pointer to sessions variables
    QList<rugeVariable> *getVariables();

    // Adds variable to the Session. Doesn't check if variable is
    // valid. If variable with the same name exist, then the old one
    // is deleted and the new one added.
    void addVariable(rugeVariable added);

    // Deletes variable with name varName.
    // If it does not exist this does nothing.
    void removeVariable(QString varName);


    // Returns the rugeVariable with the name of varName.
    // If variable doesn't exist, return empty rugeVariable.
    rugeVariable getVariable(QString varName);

    // Get the ip value. Used for getting the
    // ip address and the max ip address at the same time.
    rugeVariable getSrcIP();
    rugeVariable getDstIP();

    // Get the MAC value as string
    QString getSrcMAC();
    QString getDstMAC();

    // Get port values as strings
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
