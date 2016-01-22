#ifndef VARIABLEDATA_H
#define VARIABLEDATA_H

#include<QList>

// Datastructure for ruge variables.
struct rugeVariable{
    QString name;
    QString value;
    QString min;
    QString max;
    QString type;
    int loopIncrement;
    int increment;
    int size;
    int offset;
};

class VariableData
{
public:
    VariableData();



    bool checkVariable(rugeVariable &checked);
    rugeVariable getData(QString variableName);


private:
    QList<rugeVariable> allVariables_;

};

#endif // VARIABLEDATA_H
