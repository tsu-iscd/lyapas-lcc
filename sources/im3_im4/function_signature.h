#pragma once
#include <translation_module/translation_module.h>

class FunctionSignature {
public:
    std::string name;
    std::vector<JSON> input;
    std::vector<JSON> output;

    FunctionSignature(const JSON &cmd);
    int getNumberOfArgs();
};