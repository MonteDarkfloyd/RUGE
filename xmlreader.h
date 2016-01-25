#ifndef XMLREADER_H
#define XMLREADER_H
#include "newsessiondialog.h"
#include "variabledata.h"
#include <QFileInfo>


class xmlreader
{
public:
    xmlreader();

    QList<Session *> ReadPredefXML();
    bool safeRead;
    void giveErrorPrompt();
    bool fileExists(QString path);

};

#endif // XMLREADER_H
