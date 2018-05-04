#include "function.h"
#include <algorithm>
#include <shared_utils/assertion.h>
#include <translation_module/args_range.h>
#include "array_index.h"
#include "is_int.h"

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
    trm::Filters filters{{"definition_string", {trm::ArgsFilter::Ignore::SOME, {0}}},
                         {"asm", {trm::ArgsFilter::Ignore::ALL}},
                         {"error", {trm::ArgsFilter::Ignore::ALL}},
                         {"call", {trm::ArgsFilter::Ignore::ALL}}};

    trm::ArgsRange args{filters, cmd};
    for (auto &arg : args) {
        *arg = getSubstitute(*arg);
    }
}

void Function::calculateStackVariables()
{
    trm::Filters filters{{"definition_string", {trm::ArgsFilter::Ignore::SOME, {0}}},
                         {"asm", {trm::ArgsFilter::Ignore::ALL}},
                         {"error", {trm::ArgsFilter::Ignore::ALL}},
                         {"call", {trm::ArgsFilter::Ignore::NAME_FUNCTION_AND_SLASH}}};

    for (auto &arg : signature.input) {
        insertArg(arg);
    }

    for (auto &arg : signature.output) {
        insertArg(arg);
    }

    for (auto &&cmd : body) {
        trm::ArgsRange args{filters, cmd};

        for (auto &arg : args) {
            insertVariable(*arg);
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
        return arrayIndex.prefix + var->alias + "[" + arrayIndex.index + "]";
    };
    return arrayIndex.prefix + var->alias + "[" + index->alias + "]";
}

Function::Variables::iterator Function::findVariable(std::string nameVariable)
{
    return std::find_if(variables.begin(), variables.end(),
                        [&nameVariable](const Variable &element) { return element.name == nameVariable; });
}
