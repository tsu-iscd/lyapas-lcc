#include "array_index.h"
#include <shared_utils/assertion.h>

ArrayIndex::ArrayIndex(const std::string &var)
    : name{var.begin(), var.begin() + var.find("[")}
    , index{var.begin() + var.find("[") + 1, var.end() - 1}
{
    LCC_ASSERT(var.back() == ']');
}