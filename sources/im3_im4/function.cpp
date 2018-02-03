#include "function.h"

Function::Function(const JSON &cmd)
    : FunctionInfo(cmd.operator[](0))
{
    body = std::vector<JSON>(++cmd.begin(), cmd.end());
    //считаем переменные, которые должны лежать на стеке
    calculateStackVariables();
}

void Function::calculateStackVariables()
{
    for (auto &var : input) {
        insertVariable(var);
    }

    for (auto &var : output) {
        insertVariable(var);
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
    return std::find_if(
        variables.begin(), variables.end(),
        [&nameVariable](const std::pair<std::string, std::string> &element) { return element.first == nameVariable; });
}

JSON Function::getSubstitute(const JSON &nameVariable)
{
    //константы не заменяются
    if (nameVariable.isInt()) {
        return nameVariable;
    }

    //если обращение по индексу
    auto bracket = nameVariable.asString().find("[");
    if (bracket != std::string::npos) {
        //попробовать чз конструктор stdstring
        std::string arrayName = nameVariable.asString().substr(0, 2);
        std::string index = nameVariable.asString().substr(bracket);
        auto var = findVariable(arrayName);
        LCC_ASSERT(var != variables.end());
        return (*var).second + index;
    }

    auto var = findVariable(nameVariable.asString());
    //этот if срабатывает, когда аргумент-название функции
    if (var == variables.end()) {
        return nameVariable;
    }
    return (*var).second;
}
