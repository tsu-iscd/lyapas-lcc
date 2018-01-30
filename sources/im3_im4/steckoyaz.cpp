#include "steckoyaz.h"

JSON createStackAllocCmd(int shift)
{
    JSON command;
    command["type"] = "cmd";
    command["args"] = shift;
    command["cmd"] = "stack alloc";

    return command;
}

JSON createStackFreeCmd(int shift)
{
    JSON command;
    command["type"] = "cmd";
    command["args"] = shift;
    command["cmd"] = "stack free";

    return command;
}

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
    //обрабатываем последнюю функцию
    translateDefinition(function, intermediateCmds);

    for (auto &&cmd : intermediateCmds) {
        if (cmd["type"] == "call") {
            translateCall(cmd, resultCmds);
        } else {
            resultCmds.append(std::move(cmd));
        }
    }

    cmds = std::move(resultCmds);
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
    FunctionInfo funcInf(cmd);
    JSON addedCmd;
    for (auto &var : funcInf.input) {
        addedCmd.clear();
        addedCmd["type"] = "cmd";
        addedCmd["args"] = var;
        addedCmd["cmd"] = "push";
        resultCmds.append(addedCmd);
    }

    for (auto &var : funcInf.output) {
        addedCmd["type"] = "cmd";
        addedCmd["args"] = var;
        addedCmd["cmd"] = "push";
        resultCmds.append(addedCmd);
    }

    addedCmd["type"] = "cmd";
    addedCmd["args"] = funcInf.name;
    addedCmd["cmd"] = "call";
    resultCmds.append(addedCmd);

    for (auto i = funcInf.output.rbegin(); i != funcInf.output.rend(); i++) {
        addedCmd["type"] = "cmd";
        addedCmd["args"] = (*i);
        addedCmd["cmd"] = "pop";
        resultCmds.append(addedCmd);
    };

    //освобождаем стек
    resultCmds.append(createStackFreeCmd(funcInf.input.size()));
}
void Steckoyaz::translateDefinition(JSON &function, JSON &resultCmds)
{
    if (function.isNull()) {
        return;
    }

    //название переменной-сдвиг
    std::map<std::string, int> variables;
    FunctionInfo funcInf(function.operator[](0));

    JSON addedCmd;
    addedCmd["type"] = "label";
    addedCmd["args"] = funcInf.name;
    resultCmds.append(addedCmd);

    for (auto &var : funcInf.input) {
        variables[var.asString()] = -1;
    }

    for (auto &var : funcInf.output) {
        variables[var.asString()] = -1;
    }

    auto locals = countLocalVariables(function, variables);
    //алоцируем стек
    addedCmd.clear();
    addedCmd = createStackAllocCmd(locals);
    resultCmds.append(addedCmd);

    //высчитываем сдвиг
    int shift = variables.size() - 1;
    for (auto &var : variables) {
        var.second = shift;
        shift--;
    }

    //заменяем имена переменных на сдвиг относительно stack
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
        resultCmds.append(cmd);
    }

    //освобождаем стек
    resultCmds.append(createStackFreeCmd(locals));
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
}  // namespace syaz
