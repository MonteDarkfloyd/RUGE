#ifndef XMLWRITER_H
#define XMLWRITER_H
#include <QXmlStreamWriter>
#include "session.h"


class xmlWriter
{
public:
    xmlWriter();
    bool write();
    bool remove(int row);
    //void writeUDP(Session* newSession);
    bool add(Session *newSession);

};

#endif // XMLWRITER_H
