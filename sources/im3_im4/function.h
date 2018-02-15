#pragma once
#include <translation_module/translation_module.h>
#include "function_signature.h"
#include "variable.h"

class Function {
    using Variables = std::vector<Variable>;

public:
    Function(const JSON &cmd);
    const FunctionSignature getInfo();
    const std::vector<JSON> getBody();
    void setBody(std::vector<JSON> &newBody);
    int getVariablesCount();
    void replacerArgs(JSON &cmd);
    bool isArrayIndex(const std::string &var);

private:
    void calculateStackVariables();
    void insertVariable(JSON &var);
    JSON getSubstitute(const JSON &nameVariable);
    JSON getSubstituteArrayIndex(const std::string &nameVariable);
    Variables::iterator findVariable(std::string nameVariable);

    Variables variables;
    FunctionSignature info;
    std::vector<JSON> body;
};