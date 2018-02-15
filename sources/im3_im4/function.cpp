#include "function.h"
#include <algorithm>
#include <iostream>
#include <shared_utils/assertion.h>

Function::Function(const JSON &cmd)
    : info{cmd[0]}
    , body{++cmd.begin(), cmd.end()}
{
    //считаем переменные, которые должны лежать на стеке
    calculateStackVariables();
}

const FunctionSignature Function::getInfo()
{
    return info;
}

const std::vector<JSON> Function::getBody()
{
    return body;
}

void Function::setBody(std::vector<JSON> &newBody)
{
    body = newBody;
}

int Function::getVariablesCount()
{
    return variables.size();
}

void Function::replacerArgs(JSON &cmd)
{
    auto var = cmd["args"].begin();

    //пропустили название функции
    if (cmd["cmd"] == "call") {
        var++;
    }

    for (var; var != cmd["args"].end(); var++) {
        *var = getSubstitute(*var);
    }
}

bool Function::isArrayIndex(const std::string &var)
{
    if (var.find("[") == std::string::npos) {
        return false;
    }
    return true;
}

void Function::calculateStackVariables()
{
    for (auto &var : info.input) {
        insertVariable(var);
    }

    for (auto &var : info.output) {
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
     * если это не так - то ошибка в программе*/
    if (isArrayIndex(var.asString())) {
        return;
    }

    if (findVariable(var.asString()) == variables.end()) {
        variables.emplace_back(Variable{var.asString(), "l" + std::to_string(variables.size())});
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
    return (*var).alias;
}

JSON Function::getSubstituteArrayIndex(const std::string &nameVariable)
{
    //если обращение по индексу
    auto bracket = nameVariable.find("[");
    if (bracket != std::string::npos) {
        std::string arrayName(nameVariable.begin(), nameVariable.begin() + bracket);
        std::string index(nameVariable.begin() + bracket + 1, nameVariable.end() - 1);

        auto var = findVariable(arrayName);
        LCC_ASSERT(var != variables.end());

        auto indexAlias = findVariable(index);
        LCC_ASSERT(indexAlias != variables.end());
        return (*var).alias + "[" + (*indexAlias).alias + "]";
    }
}

Function::Variables::iterator Function::findVariable(std::string nameVariable)
{
    return std::find_if(variables.begin(), variables.end(),
                        [&nameVariable](const Variable &element) { return element.name == nameVariable; });
}
