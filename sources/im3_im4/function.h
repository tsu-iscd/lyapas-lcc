#pragma once
#include <translation_module/translation_module.h>
#include "function_info.h"

using Variables = std::vector<std::pair<std::string, std::string>>;
class Function {
public:
    const FunctionInfo getInfo();
    const std::vector<JSON> getBody();
    void setBody(std::vector<JSON> &newBody);
    int getLocals();

    Function(const JSON &cmd);
    void calculateStackVariables();
    void insertVariable(JSON &var);
    Variables::iterator findVariable(std::string nameVariable);
    JSON getSubstitute(const JSON &nameVariable);

private:
    Variables variables;
    FunctionInfo info;
    std::vector<JSON> body;
    int locals;
};