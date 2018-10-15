#pragma once
#include <string>
#include <vector>

class ArrayIndex {
public:
    ArrayIndex(const std::string &var);
    static std::vector<std::string> SplitArrayIndex(const std::string);
    static bool isArrayIndex(const std::string &var);

    std::string name;
    std::string prefix;
    std::string indexVariable;
    std::vector<std::string> indexExpression;
};