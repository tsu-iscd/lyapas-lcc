#pragma once
#include <translation_module/translation_module.h>
#include "function_info.h"

class Function : public FunctionInfo {
public:
    std::vector<JSON> body;
    int locals;
    std::vector<std::pair<std::string, std::string>> variables;

    Function(const JSON &cmd);
    void countStackVariables();
    std::vector<std::pair<std::string, std::string>>::iterator findVariable(std::string nameVariable);
    JSON getSubstitute(const JSON &nameVariable);
};