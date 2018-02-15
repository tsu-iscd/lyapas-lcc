#pragma once
#include <string>

class ArrayIndex {
public:
    ArrayIndex(const std::string &var);

    std::string arrayName;
    std::string index;
};