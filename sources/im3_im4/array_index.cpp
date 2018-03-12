#include "array_index.h"
#include <shared_utils/assertion.h>

ArrayIndex::ArrayIndex(const std::string &var)
{
    LCC_ASSERT(var.back() == ']');

    auto leftBracket = var.find('[');
    auto rightBracket = var.find(']');
    index = var.substr(leftBracket + 1, rightBracket - 1);
    prefix = "";

    auto content = var.substr(0, leftBracket);
    auto space = content.find(' ');

    if (space != std::string::npos) {
        prefix = content.substr(0, space + 1);
        content = content.substr(space + 1);
    }

    name = content;
}
