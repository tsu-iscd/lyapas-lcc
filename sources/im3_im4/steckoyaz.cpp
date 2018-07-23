#include "steckoyaz.h"
#include <algorithm>
#include <shared_utils/assertion.h>

namespace {

JSON makeCmd(const std::string &name, const std::initializer_list<JSON> &args = {})
{
    JSON res;
    res["type"] = "cmd";
    res["cmd"] = name;
    res["args"] = JSON{Json::arrayValue};
    for (const JSON &arg : args) {
        res["args"].append(arg);
    }
    return res;
}
JSON createLabel(std::string labelName)
{
    JSON command;
    command["type"] = "label";
    command["name"] = labelName;

    return command;
}
}  // namespace

std::vector<Function> parseFunctions(const JSON &cmds)
{
    std::vector<Function> program;
    auto isDefinition = [](const JSON &cmd) { return cmd["type"] == "cmd" && cmd["cmd"] == "definition"; };

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

    //транслируем definitions
    for (auto &function : program) {
        translateDefinition(function);
    }

    //транслируем calls в теле каждой функции
    for (auto &function : program) {
        translateCall(function);
    }

    //записываем результат трансляции
    cmds.clear();
    for (auto &function : program) {
        for (auto &cmd : function.getBody()) {
            cmds.append(cmd);
        }
    }
}

void Steckoyaz::postprocess(JSON &cmds) {}

trm::Replacers Steckoyaz::makeReplacers()
{
    return {};
}

std::string Steckoyaz::getRules()
{
    return std::string("");
}

void Steckoyaz::translateCall(Function &func)
{
    std::vector<JSON> resultCmds;

    unsigned counter = 0;
    auto save = [&counter, &resultCmds](const JSON &var) {
        std::string storeName = "arg" + std::to_string(counter++);
        resultCmds.push_back(makeCmd("move", {storeName, var}));
    };
    auto load = [&counter, &resultCmds](const JSON &var) {
        std::string loadName = "arg" + std::to_string(--counter);
        resultCmds.push_back(makeCmd("move", {var, loadName}));
    };

    for (auto &&cmd : func.getBody()) {
        if (cmd["cmd"] == "call") {
            FunctionSignature funcInf(cmd);
            JSON addedCmd;
            for (auto &var : funcInf.input) {
                save(var);
            }

            for (auto &var : funcInf.output) {
                save(var);
            }

            resultCmds.push_back(makeCmd("stack_alloc", {counter}));
            resultCmds.push_back(makeCmd("call", {funcInf.name}));
            resultCmds.push_back(makeCmd("stack_free", {counter}));

            for (auto it = funcInf.output.rbegin(); it != funcInf.output.rend(); it++) {
                load(*it);
            };

            continue;
        }
        resultCmds.push_back(cmd);
    }
    func.setBody(resultCmds);
}

void Steckoyaz::translateDefinition(Function &func)
{
    std::vector<JSON> resultCmds;

    resultCmds.push_back(createLabel(func.getSignature().name));

    //алоцируем стек
    resultCmds.push_back(makeCmd("stack_alloc", {static_cast<int>(func.getLocalVariablesCount())}));

    for (auto &&cmd : func.getBody()) {
        if (cmd["type"] == "label") {
            resultCmds.push_back(createLabel("_" + func.getSignature().name + "_" + cmd["number"].asString()));
            continue;
        }
        func.substituteCmdArgs(cmd);
        resultCmds.push_back(cmd);
    }

    //освобождаем стек
    resultCmds.push_back(makeCmd("stack_free", {static_cast<int>(func.getLocalVariablesCount())}));
    if (func.getSignature().name != "main") {
        resultCmds.push_back(makeCmd("ret"));
    }
    func.setBody(resultCmds);
}

}  // namespace syaz
