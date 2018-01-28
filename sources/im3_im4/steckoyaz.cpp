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

    translateDefinition(function, intermediateCmds);
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
    addedCmd = stackAlloc(input + output);
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

    //освобождаем стек
    addedCmd.clear();
    addedCmd = stackFree(input + output);
    resultCmds.append(std::move(addedCmd));
}
void Steckoyaz::translateDefinition(JSON &function, JSON &resultCmds)
{
    if (function.isNull()) {
        return;
    }

    //название переменной-сдвиг
    std::map<std::string, int> variables;
    FunctionInfo funcInf(function.operator[](0));

    Json::Value addedCmd;
    addedCmd.clear();
    addedCmd["type"] = "label";
    addedCmd["args"] = funcInf.functionName;
    resultCmds.append(std::move(addedCmd));

    for (auto &var : funcInf.input) {
        variables[var.asString()] = -1;
    }

    for (auto &var : funcInf.output) {
        variables[var.asString()] = -1;
    }


    auto locals = countLocalVariables(function, variables);
    //алоцируем стек
    addedCmd.clear();
    addedCmd = stackAlloc(locals);
    resultCmds.append(std::move(addedCmd));

    int shift = variables.size() - 1;
    for (auto &var : variables) {
        var.second = shift;
        shift--;
    }

    for (auto &&cmd : function) {
        //:))чтобы пропустить первую команду
        if (cmd["type"] == "definition") {
            continue;
        }

        for (auto &var : cmd["args"]) {
            auto pos = variables.find(var.asString());
            if (pos != variables.end()) {
                auto address = "[stack-" + std::to_string((*pos).second) + "]";
                var = address;
            }
        }
        resultCmds.append(std::move(cmd));
    }

    //освобождаем стек
    addedCmd.clear();
    addedCmd = stackFree(locals);
    resultCmds.append(std::move(addedCmd));
}

int Steckoyaz::countLocalVariables(JSON &function, std::map<std::string, int> &variables)
{
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
        for (auto &var : cmd["args"]) {
            //константы не должны лежать на стеке
            if (var.isInt()) {
                continue;
            }

            //пропустили обращение по индексу
            if (var.asString().find("[") != std::string::npos) {
                continue;
            }

            if (variables.find(var.asString()) == variables.end()) {
                variables.insert(variables.end(), std::pair<std::string, int>(var.asString(), -1));
                locals++;
            };
        }
    }

    return locals;
}

Json::Value Steckoyaz::stackAlloc(int shift)
{
    Json::Value command;
    command["type"] = "cmd";
    command["args"] = shift;
    command["cmd"] = "stack alloc";

    return command;
}

Json::Value Steckoyaz::stackFree(int shift)
{
    Json::Value command;
    command["type"] = "cmd";
    command["args"] = shift;
    command["cmd"] = "stack free";

    return command;
}

}  // namespace syaz
