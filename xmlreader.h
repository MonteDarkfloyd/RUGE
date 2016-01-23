#ifndef XMLREADER_H
#define XMLREADER_H
#include "newsessiondialog.h"


class xmlreader
{
public:
    xmlreader();
    void Read(NewSessionDialog *window);
    void Fill();
};

#endif // XMLREADER_H
