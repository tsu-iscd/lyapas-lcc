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

template <class T>
class Debug;

JSON Function::getSubstitute(const JSON &nameVariable)
{
    //константы не заменяются
    if (nameVariable.isInt()) {
        return nameVariable;
    }

    std::string nameVar{nameVariable.asString()};
    //если обращение по индексу
    auto bracket = nameVar.find("[");
    if (bracket != std::string::npos) {
        std::string arrayName(nameVar.begin(), nameVar.begin() + bracket);
        std::string index(nameVar.begin() + bracket, nameVar.end());
        auto var = findVariable(arrayName);
        LCC_ASSERT(var != variables.end());
        return (*var).second + index;
    }

    auto var = findVariable(nameVariable.asString());
    //этот if срабатывает, когда аргумент-название функции
    //FIXME есть случаи, когда название функции совпадет с переменной
    if (var == variables.end()) {
        return nameVariable;
    }
    return (*var).second;
}
