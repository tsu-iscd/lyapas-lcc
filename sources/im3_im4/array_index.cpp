#include "array_index.h"
#include <shared_utils/assertion.h>

ArrayIndex::ArrayIndex(const std::string &var)
{
    LCC_ASSERT(var.back() == ']');

    index = std::string{var.begin() + var.find("[") + 1, var.end() - 1};

    //если есть префикс
    if (var.find(" ") == std::string::npos) {
        name = std::string{var.begin(), var.begin() + var.find("[")};
        prefix = std::string{""};
        return;
    }

    name = std::string{var.begin() + var.find(" ") + 1, var.begin() + var.find("[")};
    prefix = std::string{var.begin(), var.begin() + var.find(" ") + 1};
}
