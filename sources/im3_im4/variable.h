#pragma once
#include <ctype.h>

struct Variable {
    std::string name;
    std::string alias;
};

static bool isInt(std::string nameVar)
{
    for (auto &letter : nameVar) {
        if (!isdigit(letter)) {
            return false;
        };
    }
    return true;
}
