#include "function.h"
#include <iostream>

Function::Function(const JSON &cmd)
    : FunctionInfo(cmd.operator[](0))
{
    auto l = cmd.begin();
    l++;
    body = std::vector<JSON>(l, cmd.end());
    //считаем переменные, которые должны лежать на стеке
    countStackVariables();
}

void Function::countStackVariables()
{
    for (auto &var : input) {
        variables.emplace_back(var.asString(), "l" + std::to_string(variables.size()));
    }

    for (auto &var : output) {
        variables.emplace_back(var.asString(), "l" + std::to_string(variables.size()));
    }

    for (auto &&cmd : body) {
        if (cmd["type"] == "call") {
            FunctionInfo funcInf(cmd);
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
    if (var.asString().find("[") != std::string::npos) {
        return;
    }

    if (findVariable(var.asString()) == variables.end()) {
        variables.emplace_back(var.asString(), "l" + std::to_string(variables.size()));
        locals++;
    }
}

std::vector<std::pair<std::string, std::string>>::iterator Function::findVariable(std::string nameVariable)
{
    for (auto var = variables.begin(); var != variables.end(); var++) {
        if ((*var).first == nameVariable)
            return var;
    }
    return variables.end();
}

JSON Function::getSubstitute(const JSON &nameVariable)
{
    if (nameVariable.isInt()) {
        return nameVariable;
    }

    auto bracket = nameVariable.asString().find("[");
    if (bracket != std::string::npos) {
        std::string arrayName = nameVariable.asString().substr(0, 2);
        std ::string index = nameVariable.asString().substr(bracket);
        return (*findVariable(arrayName)).second + index;
    }

    auto it = findVariable(nameVariable.asString());
    if (it == variables.end())
        return nameVariable;
    return (*it).second;
}
