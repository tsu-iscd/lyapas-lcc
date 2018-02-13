#pragma once
#include <translation_module/translation_module.h>
#include "function_info.h"
#include "variable.h"

using Variables = std::vector<Variable>;

class Function {
public:
    Function(const JSON &cmd);
    const FunctionInfo getInfo();
    const std::vector<JSON> getBody();
    void setBody(std::vector<JSON> &newBody);
    int getVariablesCount();
    JSON getSubstitute(const JSON &nameVariable);

private:
    void calculateStackVariables();
    void insertVariable(JSON &var);
    Variables::iterator findVariable(std::string nameVariable);

    Variables variables;
    FunctionInfo info;
    std::vector<JSON> body;
};