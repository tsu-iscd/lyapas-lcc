#pragma once
#include <translation_module/translation_module.h>
#include "function_info.h"

struct Variable {
    std::string name;
    std::string alias;
};

using Variables = std::vector<Variable>;

class Function {
public:
    const FunctionInfo getInfo();
    const std::vector<JSON> getBody();
    void setBody(std::vector<JSON> &newBody);
    int getVariablesCount();

    Function(const JSON &cmd);
    void calculateStackVariables();
    void insertVariable(JSON &var);
    Variables::iterator findVariable(std::string nameVariable);
    JSON getSubstitute(const JSON &nameVariable);

private:
    Variables variables;
    FunctionInfo info;
    std::vector<JSON> body;
};