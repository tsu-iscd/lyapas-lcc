#include <cctype>

bool isInt(std::string nameVar)
{
    for (auto &letter : nameVar) {
        if (!isdigit(letter)) {
            return false;
        };
    }
    return true;
}
