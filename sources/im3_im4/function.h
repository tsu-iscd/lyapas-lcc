#pragma once
#include <translation_module/translation_module.h>
#include "function_info.h"
#include "variable.h"

class Function {
    using Variables = std::vector<Variable>;

public:
    Function(const JSON &cmd);
    const FunctionInfo getInfo();
    const std::vector<JSON> getBody();
    void setBody(std::vector<JSON> &newBody);
    int getVariablesCount();
    void replacer(JSON &cmd);
    JSON getSubstitute(const JSON &nameVariable);
    JSON getSubstituteArrayIndex(const std::string &nameVariable);
    bool isArrayIndex(const std::string &var);

private:
    void calculateStackVariables();
    void insertVariable(JSON &var);
    Variables::iterator findVariable(std::string nameVariable);

    Variables variables;
    FunctionInfo info;
    std::vector<JSON> body;
};