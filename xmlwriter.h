#ifndef XMLWRITER_H
#define XMLWRITER_H
#include <QXmlStreamWriter>
#include "session.h"


class xmlWriter
{
public:
    xmlWriter();
    void write();
    void edit(Session *newSession);
    void writeUDP(Session* newSession);
};

#endif // XMLWRITER_H
