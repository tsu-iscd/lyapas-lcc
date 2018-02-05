#pragma once
#include <translation_module/translation_module.h>
#include "function_info.h"

class Function {
private:
    std::vector<std::pair<std::string, std::string>> variables;

public:
    FunctionInfo info;
    std::vector<JSON> body;
    int locals;

    Function(const JSON &cmd);
    void calculateStackVariables();
    void insertVariable(JSON &var);
    std::vector<std::pair<std::string, std::string>>::iterator findVariable(std::string nameVariable);
    JSON getSubstitute(const JSON &nameVariable);
};