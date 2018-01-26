#include "steckoyaz.h"
#include <iostream>

namespace syaz {
bool Steckoyaz::valid(const JSON &cmds, std::string &errror)
{
    return true;
}

void Steckoyaz::preprocess(JSON &cmds)
{
    JSON resultCmds;
    JSON function;
    JSON intermediateCmds;

    for (auto &&cmd : cmds) {
        if (cmd["type"] == "definition") {
            translateDefinition(function, intermediateCmds);
            function.clear();
        }
        function.append(std::move(cmd));
    }
    /*for (auto &&cmd : cmds) {
        if (cmd["type"] == "call") {
            translateCall(cmd, resultCmds);
        } else if (cmd["type"] == "definition"){
            translateDefinition(cmd, resultCmds);
        } else {
            resultCmds.append(std::move(cmd));
        }
    }*/

    cmds = std::move(intermediateCmds);
}

void Steckoyaz::postprocess(JSON &cmds) {}

const trm::Replacers &Steckoyaz::getReplacers(const JSON &cmds)
{
    static trm::Replacers replacers;
    return replacers;
}

std::string Steckoyaz::getRules()
{
    return std::string("");
}

void Steckoyaz::translateCall(const JSON &cmd, JSON &resultCmds)
{
    int input, output;
    input = output = 0;

    //считаем количество входных/выходных параметров
    auto i = cmd["args"].begin();
    auto function_name = (*i);
    i++;

    for (i; (*i) != "/"; i++) {
        input++;
    }

    //пропускаем "/"
    for (i++; i != cmd["args"].end(); i++) {
        output++;
    }

    std::cout << "input " << input << " output " << output << std::endl;

    Json::Value addedCmd;
    addedCmd.clear();

    //алоцируем стек
    addedCmd["type"] = "cmd";
    addedCmd["args"] = input + output;
    addedCmd["cmd"] = "stack alloc";
    resultCmds.append(std::move(addedCmd));

    for (int i = 1; i <= input; i++) {
        addedCmd.clear();
        addedCmd["type"] = "cmd";

        int shift = input + output + i;

        auto first_arg = "[stack-" + std::to_string(shift) + "]";
        auto second_arg = "[" + cmd["args"].operator[](i).asString() + "]";
        addedCmd["args"].append(first_arg);
        addedCmd["args"].append(second_arg);

        addedCmd["cmd"] = "move";
        resultCmds.append(std::move(addedCmd));
    }

    addedCmd.clear();
    addedCmd["type"] = "cmd";
    addedCmd["args"] = function_name;
    addedCmd["cmd"] = "call";
    resultCmds.append(std::move(addedCmd));

    for (int i = 1; i <= output; i++) {
        addedCmd.clear();
        addedCmd["type"] = "cmd";

        int shift = input + 1;

        auto first_arg = "[" + cmd["args"].operator[](input + 1 + i).asString() + "]";
        auto second_arg = "[stack-" + std::to_string(shift) + "]";
        addedCmd["args"].append(first_arg);
        addedCmd["args"].append(second_arg);

        addedCmd["cmd"] = "move";
        resultCmds.append(std::move(addedCmd));
    }

    addedCmd.clear();

    //освобождаем стек
    addedCmd["type"] = "cmd";
    addedCmd["args"] = input + output;
    addedCmd["cmd"] = "stack free";
    resultCmds.append(std::move(addedCmd));
}
void Steckoyaz::translateDefinition(const JSON &function, JSON &resultCmds)
{
    if (function.isNull()) {
        return;
    }

    std::cout << "========================" << std::endl;
    std::cout << function << std::endl;
    std::cout << "========================" << std::endl;

    // TODO посчитать локальные переменные
    int input, output;
    input = output = 0;
    //название переменной-сдвиг
    std::map<std::string, int> variables;

    //считаем количество входных/выходных параметров
    auto i = function.operator[](0)["args"].begin();
    auto function_name = (*i);
    i++;

    for (i; (*i) != "/"; i++) {
        variables[(*i).asString()] = -1;
        input++;
    }

    //пропускаем "/"
    for (i++; i != function.operator[](0)["args"].end(); i++) {
        variables[(*i).asString()] = -1;
        output++;
    }

    std::cout << "input " << input << " output " << output << std::endl;

    int locals = 0;
    for (auto &&cmd : function) {
        //:)для того, чтобы пропустить первый
        if (cmd["type"] == "definition") {
            continue;
        }

        //"call" должны пропускать
        if (cmd["type"] == "call") {
            continue;
        }

        //считаем количество локальных, записываем их название
        //TODO отличать индексы массивов
        for (auto i = cmd["args"].begin(); i != cmd["args"].end(); i++) {
            //константы не должны лежать на стеке
            if ((*i).isInt()) {
                continue;
            }

            if (variables.find((*i).asString()) == variables.end()) {
                variables.insert(variables.end(), std::pair<std::string, int>((*i).asString(), -1));
                std::cout << (*i) << std::endl;
                locals++;
            };
        }

    }

    std::cout << "locals " << locals << std::endl;

    for (auto &&cmd : function) {
        resultCmds.append(std::move(cmd));
    }
}

}  // namespace syaz
