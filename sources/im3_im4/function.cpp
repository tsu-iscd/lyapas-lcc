#include "function.h"
#include <algorithm>
#include <shared_utils/assertion.h>
#include "array_index.h"

Function::Function(const JSON &cmd)
    : signature{cmd[0]}
    , body{++cmd.begin(), cmd.end()}
{
    //считаем переменные, которые должны лежать на стеке
    calculateStackVariables();
}

const FunctionSignature Function::getSignature()
{
    return signature;
}

std::vector<JSON> &Function::getBody()
{
    return body;
}

void Function::setBody(std::vector<JSON> &newBody)
{
    body = newBody;
}

size_t Function::getVariablesCount()
{
    return variables.size();
}

void Function::substituteCmdArgs(JSON &cmd)
{
    auto var = cmd["args"].begin();

    //название функции не транслируем
    if (cmd["cmd"] == "call") {
        return;
    }

    for (var; var != cmd["args"].end(); var++) {
        *var = getSubstitute(*var);
    }
}

void Function::calculateStackVariables()
{
    for (auto &var : signature.input) {
        insertVariable(var);
    }

    for (auto &var : signature.output) {
        insertVariable(var);
    }

    for (auto &&cmd : body) {
        if (cmd["type"] == "call") {
            FunctionSignature funcInf(cmd);
            for (auto &var : funcInf.input) {
                insertVariable(var);
            }
            for (auto &var : funcInf.output) {
                insertVariable(var);
            }
            continue;
        }

        for (auto &var : cmd["args"]) {
            insertVariable(var);
        }
    }
}

void Function::insertVariable(JSON &var)
{
    //константы не лежат на стеке
    if (var.isInt()) {
        return;
    }

    /*пропустили обращение по индеку, название комплекса точно уже на стеке
     * либо объявили в функции, либо комплекс пришел в аргументах
     * если это не так - то ошибка в программе. индекc не факт, что лежит на стеке*/
    auto nameVar = var.asString();
    if (isArrayIndex(nameVar)) {
        ArrayIndex arrayIndex(nameVar);
        nameVar = arrayIndex.index;
    }

    if (findVariable(nameVar) == variables.end()) {
        variables.emplace_back(Variable{nameVar, "l" + std::to_string(variables.size())});
    }
}

JSON Function::getSubstitute(const JSON &nameVariable)
{
    //константы не заменяются
    if (nameVariable.isInt()) {
        return nameVariable;
    }

    std::string nameVar{nameVariable.asString()};

    //если обращение по индексу
    if (isArrayIndex(nameVar)) {
        return getSubstituteArrayIndex(nameVar);
    }

    auto var = findVariable(nameVar);
    LCC_ASSERT(var != variables.end());
    return var->alias;
}

JSON Function::getSubstituteArrayIndex(const std::string &nameVariable)
{
    ArrayIndex arrayIndex(nameVariable);
    auto var = findVariable(arrayIndex.arrayName);
    LCC_ASSERT(var != variables.end());

    auto index = findVariable(arrayIndex.index);
    LCC_ASSERT(index != variables.end());
    return var->alias + "[" + index->alias + "]";
}

Function::Variables::iterator Function::findVariable(std::string nameVariable)
{
    return std::find_if(variables.begin(), variables.end(),
                        [&nameVariable](const Variable &element) { return element.name == nameVariable; });
}

bool Function::isArrayIndex(const std::string &var)
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
