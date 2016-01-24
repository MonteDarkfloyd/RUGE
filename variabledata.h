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

    // Checks variable name, type, size, increment, max and min.
    // Used when loading session from an xml file and checking
    // the variables from the beginning. If value is legal this function will
    // add the correct offset to the checked Variable.
    // Return value false if not valid variable.
    bool checkVariable(rugeVariable &checked);

    // Returns the variable that has same name as variableName.
    // If not found returns empty rugeVariable.
    rugeVariable getData(QString variableName);


private:
    QList<rugeVariable> allVariables_;

};

#endif // VARIABLEDATA_H
