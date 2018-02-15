#include "array_index.h"

ArrayIndex::ArrayIndex(const std::string &var)
    : arrayName{var.begin(), var.begin() + var.find("[")}
    , index{var.begin() + var.find("[") + 1, var.end() - 1}
{
}