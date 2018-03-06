#pragma once
#include <string>

class ArrayIndex {
public:
    ArrayIndex(const std::string &var);

    std::string name;
    std::string prefix;
    std::string index;
};