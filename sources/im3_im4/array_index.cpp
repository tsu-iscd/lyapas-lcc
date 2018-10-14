#include "array_index.h"
#include "is_int.h"
#include <algorithm>
#include <shared_utils/assertion.h>

inline bool space(char c){
    return std::isspace(c);
}

inline bool notspace(char c){
    return !std::isspace(c);
}

std::vector<std::string> ArrayIndex::SplitArrayIndex(std::string index) {
    std::vector<std::string> ret;
    auto i = index.begin();
    while(i!=index.end()){
        i = std::find_if(i, index.end(), notspace); // find the beginning of a word
        auto j= std::find_if(i, index.end(), space); // find the end of the same word
        if(i!=index.end()){
            ret.push_back(std::string(i, j)); //insert the word into vector
            i = j; // repeat 1,2,3 on the rest of the line.
        }
    }
    return ret;
}

ArrayIndex::ArrayIndex(const std::string &var)
{
    LCC_ASSERT(isArrayIndex(var));

    auto leftBracket = var.rfind('[');
    auto rightBracket = var.rfind(']');

    auto expression = var.substr(leftBracket + 1, rightBracket - leftBracket - 1);

    indexExpression = SplitArrayIndex(expression);

    for(auto iter = indexExpression.begin(); iter < indexExpression.end(); iter++) {
        if (isInt(*iter)) {
            continue;
        }
        if (*iter == "-") {
            continue;
        }
        if (*iter == "+") {
            continue;
        }
        if (*iter == "*") {
            continue;
        }
        else indexVariable = *iter;
        break;
    }


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
