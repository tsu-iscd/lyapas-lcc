#pragma once
#include <translation_module/translation_module.h>

class FunctionInfo {
    public:
        std::string functionName;
        std::vector<Json::Value> input;
        std::vector<Json::Value> output;

    FunctionInfo(const JSON &cmd);
};