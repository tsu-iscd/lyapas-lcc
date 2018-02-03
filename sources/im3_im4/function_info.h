#pragma once
#include <translation_module/translation_module.h>

class FunctionInfo {
public:
    std::string name;
    std::vector<JSON> input;
    std::vector<JSON> output;

    FunctionInfo(const JSON &cmd);
};