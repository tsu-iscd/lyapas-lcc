#pragma once
#include <translation_module/translation_module.h>
#include "function_signature.h"
#include "variable.h"

class Function {
    using Variables = std::vector<Variable>;

public:
    Function(const JSON &cmd);
    const FunctionSignature getSignature();
    std::vector<JSON> &getBody();
    void setBody(std::vector<JSON> &newBody);
    size_t getVariablesCount();
    void substituteCmdArgs(JSON &cmd);

private:
    void calculateStackVariables();
    void insertArg(JSON &var);
    void insertVariable(std::string nameVar);
    JSON getSubstitute(const JSON &nameVariable);
    JSON getSubstituteArrayIndex(const std::string &nameVariable);
    Variables::iterator findVariable(std::string nameVariable);

    Variables variables;
    FunctionSignature signature;
    std::vector<JSON> body;
};