#include "function.h"
#include <algorithm>
#include <shared_utils/assertion.h>
#include <translation_module/args_range.h>
#include <shared_utils/array_index.h>
#include <shared_utils/is_int.h>

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

size_t Function::getLocalVariablesCount()
{
    return variables.size() - signature.getNumberOfArgs();
}

void Function::substituteCmdArgs(JSON &cmd)
{
    trm::Filters filters{{"definition_string", {trm::ArgsFilter::Ignore::SOME, {0}}},
                         {"asm", {trm::ArgsFilter::Ignore::ALL}},
                         {"error", {trm::ArgsFilter::Ignore::ALL}},
                         {"call", {trm::ArgsFilter::Ignore::NAME_FUNCTION_AND_SLASH}}};

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

    //
    // сдвиг переменных p<N>
    //
    // пример, содержание variables до:
    // l0, l1, l2, p0, p1, p2
    //
    // поле:
    // l0, l1, l2, p4, p5, p6
    //
    // таким образом переменные легко кладутся на стек:
    //  __________
    // |____p6____|
    // |____p5____|
    // |____p4____|
    // |_ret.addr.|
    // |____l2____|
    // |____l1____|
    // |____l0____|
    // |          |
    //
    for (auto &var : variables) {
        if (var.alias[0] == 'p') {
            std::string numStr = var.alias.substr(1);
            auto num = variables.size() - std::stoull(numStr);
            var.alias = 'p' + std::to_string(num);
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
        nameVar = arrayIndex.indexVariable;
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


    std::string result = arrayIndex.prefix + var->alias + "[";
    for (auto iter = arrayIndex.indexExpression.begin(); iter < arrayIndex.indexExpression.end(); iter++) {
        auto indexPart = findVariable(*iter);
        if (indexPart == variables.end()) {
            result += (*iter) + " ";
            continue;
        };
        result += indexPart->alias + " ";
    }
    result = result.substr(0, result.size()-1) + "]";
    return result;
}

Function::Variables::iterator Function::findVariable(std::string nameVariable)
{
    return std::find_if(variables.begin(), variables.end(),
                        [&nameVariable](const Variable &element) { return element.name == nameVariable; });
}
