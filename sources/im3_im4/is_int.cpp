#include "is_int.h"

bool isInt(const std::string &nameVar)
{
    for (auto &letter : nameVar) {
        if (!isdigit(letter)) {
            return false;
        };
    }
    return true;
}
