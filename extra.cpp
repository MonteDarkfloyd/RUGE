#include "extra.h"

void addXML(QString &name){
    if(name.endsWith(".xml",Qt::CaseSensitive)){
        return;
    }
    else{
        name.append(".xml");
    }
return;
}
