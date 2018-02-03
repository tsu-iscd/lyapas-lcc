#pragma once
#include <algorithm>
#include <shared_utils/assertion.h>
#include <translation_module/translation_module.h>
/*поправить инклуды, в h только для интерфейса*/

class FunctionInfo {
public:
    std::string name;
    std::vector<JSON> input;
    std::vector<JSON> output;

    FunctionInfo(const JSON &cmd);
};