#include "array_index.h"
#include <algorithm>
#include <shared_utils/assertion.h>

ArrayIndex::ArrayIndex(const std::string &var)
{
    LCC_ASSERT(isArrayIndex(var));

    auto leftBracket = var.rfind('[');
    auto rightBracket = var.rfind(']');
    index = var.substr(leftBracket + 1, rightBracket - 1);

    auto content = var.substr(0, leftBracket);
    auto space = content.find(' ');

    if (space != std::string::npos) {
        prefix = content.substr(0, space + 1);
        content = content.substr(space + 1);
    }

    name = content;
}

bool ArrayIndex::isArrayIndex(const std::string &var)
{
    if (var.find('[') == std::string::npos) {
        return false;
    }

    if (std::count(var.begin(), var.end(), '[') > 1) {
        throw std::runtime_error("Повторяющийся символ '[':" + var);
    }

    if (var.back() != ']') {
        throw std::runtime_error("']' не последний символ в имени переменной:" + var);
    }

    if (std::count(var.begin(), var.end(), ']') > 1) {
        throw std::runtime_error("Повторяющийся символ ']':" + var);
    }

    return true;
}
