#include <shared_utils/assertion.h>
#include "function.h"
#include <iostream>

Function::Function(const JSON &cmd)
{
    const JSON &args = cmd[0]["args"];
    LCC_ASSERT(args.size() != 0);

    name = args.operator[](0).asString();

    auto slash = std::find(args.begin(), args.end(), "/");
    LCC_ASSERT(slash != args.end());
    auto s = args.begin();
    s++;
    input = std::vector<JSON>(s, slash);
    slash++;
    output = std::vector<JSON>(slash, args.end());

    auto l = cmd.begin();
    l++;
    body = std::vector<JSON>(l, cmd.end());
    countStackVariables();
}

void Function::countStackVariables()
{
    for(auto &var: input) {
        variables.emplace_back(var.asString(), "l"+std::to_string(variables.size()));
    }

    for(auto &var: output) {
        variables.emplace_back(var.asString(), "l"+std::to_string(variables.size()));
    }

    for (auto &&cmd : body) {
        //:)для того, чтобы пропустить первый
        if (cmd["type"] == "definition") {
            continue;
        }

        //"call" должны пропускать
        if (cmd["type"] == "call") {
            FunctionInfo funcInf(cmd);
            for(auto &var : funcInf.input) {
                if (var.isInt()) continue;
                if (findVariable(var.asString()) == variables.end()) {
                variables.emplace_back(var.asString(), "l"+std::to_string(variables.size()));
                locals++;
                };
            }
            for(auto &var : funcInf.output) {
                if (var.isInt()) continue;
                if (findVariable(var.asString()) == variables.end()) {
                    variables.emplace_back(var.asString(), "l"+std::to_string(variables.size()));
                    locals++;
                };
            }
            continue;
        }

        for (auto &var : cmd["args"]) {
            //константы не должны лежать на стеке
            if (var.isInt()) {
                continue;
            }

            //пропустили обращение по индексу
            if (var.asString().find("[") != std::string::npos) {
                continue;
            }

            if (findVariable(var.asString()) == variables.end()) {
                variables.emplace_back(var.asString(), "l"+std::to_string(variables.size()));
                locals++;
            };
        }
    }
}

std::vector<std::pair<std::string, std::string>>::iterator Function::findVariable(std::string nameVariable) {
    for(auto var = variables.begin(); var != variables.end(); var++) {
        if((*var).first == nameVariable) return var;
    }
    return variables.end();
}

JSON Function::getSubstitute(const JSON &nameVariable)
{
    if(nameVariable.isInt()) {
        return nameVariable;
    }

    auto bracket = nameVariable.asString().find("[");
    if(bracket != std::string::npos) {
        std::string arrayName = nameVariable.asString().substr(0, 2);
        std :: string index = nameVariable.asString().substr(bracket);
        return (*findVariable(arrayName)).second + index;
    }

    auto it = findVariable(nameVariable.asString());
    if (it == variables.end()) return nameVariable;
    return (*it).second;
}


