#include "steckoyaz.h"
#include <iostream>
#include <shared_utils/assertion.h>

JSON createCmd(JSON args, std::string cmd)
{
    JSON command;
    command["type"] = "cmd";
    command["args"].append(args);
    command["cmd"] = cmd;

    return command;
}

std::vector<Function> parseFunctions(const JSON &cmds)
{
    std::vector<Function> program;
    auto isDefinition = [](const JSON &cmd) { return cmd["type"] == "definition"; };

    auto current = cmds.begin();
    auto end = cmds.end();
    while (current != end) {
        LCC_ASSERT(isDefinition(*current));
        auto begin = current;
        current = std::find_if(++current, end, isDefinition);

        JSON function;
        std::for_each(begin, current, [&function](const JSON &cmd) { function.append(cmd); });
        program.emplace_back(function);
    }

    return program;
}

namespace syaz {
bool Steckoyaz::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Steckoyaz::preprocess(JSON &cmds)
{
    //разбиваем все на функции
    auto program = parseFunctions(cmds);

    //транслируем calls в теле каждой функции
    for (auto &function : program) {
        translateCall(function);
    }

    //транслируем definitions
    cmds.clear();
    for (auto &function : program) {
        translateDefinition(function, cmds);
    }
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

void Steckoyaz::translateCall(Function &func)
{
    std::vector<JSON> resultCmds;
    for (auto &&cmd : func.body) {
        if (cmd["type"] == "call") {
            FunctionInfo funcInf(cmd);
            JSON addedCmd;
            for (auto &var : funcInf.input) {
                resultCmds.push_back(createCmd(var, "push"));
            }

            for (auto &var : funcInf.output) {
                resultCmds.push_back(createCmd(var, "push"));
            }

            resultCmds.push_back(createCmd(funcInf.name, "call"));

            for (auto i = funcInf.output.rbegin(); i != funcInf.output.rend(); i++) {
                resultCmds.push_back(createCmd((*i), "pop"));
            };

            //освобождаем стек
            resultCmds.push_back(createCmd(JSON{static_cast<int>(funcInf.input.size())}, "stack free"));
            continue;
        }
        resultCmds.push_back(cmd);
    }
    func.body = resultCmds;
}

void Steckoyaz::translateDefinition(Function &func, JSON &resultCmds)
{
    JSON addedCmd;
    addedCmd["type"] = "label";
    addedCmd["args"] = func.name;
    resultCmds.append(addedCmd);

    //алоцируем стек
    resultCmds.append(createCmd(JSON{func.locals}, "stack alloc"));

    for (auto &&cmd : func.body) {
        for (auto &var : cmd["args"]) {
            var = func.getSubstitute(var);
        }
        resultCmds.append(cmd);
    }

    //освобождаем стек
    resultCmds.append(createCmd(JSON{func.locals}, "stack free"));
}

}  // namespace syaz
