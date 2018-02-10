#include "steckoyaz.h"
#include <algorithm>
#include <shared_utils/assertion.h>

JSON createCmd(std::string cmd, JSON args)
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
    for (auto &function : program) {
        translateDefinition(function);
    }

    //записываем результат трансляции
    cmds.clear();
    for (auto &function : program) {
        for (auto &cmd : function.body) {
            cmds.append(cmd);
        }
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
                resultCmds.push_back(createCmd("push", var));
            }

            for (auto &var : funcInf.output) {
                resultCmds.push_back(createCmd("push", var));
            }

            resultCmds.push_back(createCmd("call", funcInf.name));

            for (auto it = funcInf.output.rbegin(); it != funcInf.output.rend(); it++) {
                resultCmds.push_back(createCmd("pop", (*it)));
            };

            //освобождаем стек
            resultCmds.push_back(createCmd("stack_free", JSON{static_cast<int>(funcInf.input.size())}));
            continue;
        }
        resultCmds.push_back(cmd);
    }
    func.body = resultCmds;
}

void Steckoyaz::translateDefinition(Function &func)
{
    std::vector<JSON> resultCmds;
    JSON addedCmd;

    addedCmd["type"] = "label";
    addedCmd["name"] = func.info.name;
    resultCmds.push_back(addedCmd);

    //алоцируем стек
    resultCmds.push_back(createCmd("stack_alloc", JSON{func.locals}));

    for (auto &&cmd : func.body) {
        for (auto &var : cmd["args"]) {
            var = func.getSubstitute(var);
        }
        resultCmds.push_back(cmd);
    }

    //освобождаем стек
    resultCmds.push_back(createCmd("stack_free", JSON{func.locals}));
    func.body = resultCmds;
}

}  // namespace syaz
