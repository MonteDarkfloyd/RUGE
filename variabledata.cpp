#include "variabledata.h"

VariableData::VariableData()
{
    //  name, value, min, max, type, loopIncrement, increment, size, offset
    rugeVariable IPsrc = {"IP_SRC","","0","","Binary",0,0,4,26};
    rugeVariable IPdst = {"IP_DST","","0","","Binary",0,0,4,30};
    rugeVariable MACsrc = {"MAC_SRC","","0","","Binary",0,0,6,6};
    rugeVariable MACdst = {"MAC_DST","","0","","Binary",0,0,6,0};
    rugeVariable UDPSrcPort = {"UDP_SRC_PORT","","0","","Binary",0,0,2,34};
    rugeVariable UDPDstPort = {"UDP_DST_PORT","","0","","Binary",0,0,2,36};

    // Add all known variables to a QList.
    allVariables_ << IPsrc << IPdst << MACsrc << MACdst << UDPSrcPort << UDPDstPort;

}

// Checks variable name, type, size, increment, max and min.
// Used when loading session from an xml file and checking
// the variables from the beginning. If value is legal this function will
// add the correct offset to the checked Variable.
bool VariableData::checkVariable(rugeVariable &checked){

    for(int i = 0; i < allVariables_.size(); i++){

        // Check that name, type and size match to known variable.
        if(checked.name == allVariables_.at(i).name && checked.type == allVariables_.at(i).type && checked.size == allVariables_.at(i).size){
            // Check if increment is either 1 and there is max and min, or increment is 0 and there is no max and min=0
            if(checked.increment == 1 && checked.min == checked.value && checked.max != ""){
                checked.offset = allVariables_.at(i).offset;
                return true;
            }
            else if(checked.increment == 0 && checked.min == "0" && checked.max == ""){
                checked.offset = allVariables_.at(i).offset;
                return true;
            }
            else{
                return false;
            }
        }
    }

    return false;
}

// Returns the variable that has same name as variableName.
// If not found returns empty rugeVariable.
rugeVariable VariableData::getData(QString variableName){
    for(int i = 0; i < allVariables_.size(); i++){
        if(allVariables_.at(i).name == variableName){
            return allVariables_.at(i);
        }
    }
    rugeVariable empty = {"","","","","",0,0,0,0};
    return empty;
}
