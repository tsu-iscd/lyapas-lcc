#include "is_int.h"
#include <cctype>

bool isInt(const std::string &nameVar)
{
    for (auto &letter : nameVar) {
        if (!isdigit(letter)) {
            return false;
        };
    }
    return true;
}
