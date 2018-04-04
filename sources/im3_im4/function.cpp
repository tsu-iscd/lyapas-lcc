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

    /*на данном этапе, вызовы функций оттранслированы,
     * поэтому пропускаем команды call, тк в их аргументах
     * только названия функций*/
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
        insertArg(var);
    }

    for (auto &var : signature.output) {
        insertArg(var);
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

void Function::insertArg(JSON &var)
{
    auto nameVar = var.asString();

    LCC_ASSERT(findVariable(nameVar) == variables.end());
    variables.emplace_back(Variable{nameVar, "p" + std::to_string(variables.size())});
}

void Function::insertVariable(JSON &var)
{
    auto nameVar = var.asString();

    /*ТРАНЛЯЦИЯ F1[t1]
     * имя комплекса ТОЧНО в списке локальных переменных
     * индекс может не быть в списке локальных переменных*/

    if (ArrayIndex::isArrayIndex(nameVar)) {
        ArrayIndex arrayIndex(nameVar);
        LCC_ASSERT(findVariable(arrayIndex.name) != variables.end());
        nameVar = arrayIndex.index;
    }

    //константы не лежат на стеке
    if (isInt(nameVar)) {
        return;
    }

    if (findVariable(nameVar) == variables.end()) {
        variables.emplace_back(Variable{nameVar, "l" + std::to_string(variables.size() - signature.getNumberOfArgs())});
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
    if (ArrayIndex::isArrayIndex(nameVar)) {
        return getSubstituteArrayIndex(nameVar);
    }

    auto var = findVariable(nameVar);
    LCC_ASSERT(var != variables.end());
    return var->alias;
}

JSON Function::getSubstituteArrayIndex(const std::string &nameVariable)
{
    ArrayIndex arrayIndex(nameVariable);
    auto var = findVariable(arrayIndex.name);
    LCC_ASSERT(var != variables.end());

    auto index = findVariable(arrayIndex.index);
    if (index == variables.end()) {
        LCC_ASSERT(isInt(arrayIndex.index));
        return var->alias + "[" + arrayIndex.index + "]";
    };
    return arrayIndex.prefix + var->alias + "[" + index->alias + "]";
}

Function::Variables::iterator Function::findVariable(std::string nameVariable)
{
    return std::find_if(variables.begin(), variables.end(),
                        [&nameVariable](const Variable &element) { return element.name == nameVariable; });
}
