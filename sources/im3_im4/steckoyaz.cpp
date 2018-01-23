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
    for (auto &&cmd : cmds) {
            if(cmd["type"] == "call") {
                translateCall(cmd, resultCmds);
            }
            else {
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
    int input, output;
    input = output = 0;

    //считаем количество входных/выходных параметров
    auto i = cmd["args"].begin();
    auto function_name = (*i);
    i++;

    for(i;(*i) != "/"; i++) {
        input++;
    }

    //пропускаем "/"
    for(i++; i != cmd["args"].end(); i++) {
        output++;
    }

    std::cout <<  "input " << input << " output " << output << std::endl;

    Json::Value addedCmd;
    addedCmd.clear();

    //алоцируем стек
    addedCmd["type"] = "cmd";
    addedCmd["args"] = input+output;
    addedCmd["cmd"] = "stack alloc";
    resultCmds.append(std::move(addedCmd));


    for(int i = 1; i <= input; i++) {
        addedCmd.clear();
        addedCmd["type"] = "cmd";

        int shift = input+output+i;

        auto first_arg = "[stack-" + std::to_string(shift)+"]";
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

    for(int i = 1; i <= output; i++) {
        addedCmd.clear();
        addedCmd["type"] = "cmd";

        int shift = input+1;

        auto first_arg = "[" + cmd["args"].operator[](input+1+i).asString() + "]";
        auto second_arg = "[stack-" + std::to_string(shift)+"]";
        addedCmd["args"].append(first_arg);
        addedCmd["args"].append(second_arg);

        addedCmd["cmd"] = "move";
        resultCmds.append(std::move(addedCmd));
    }

    addedCmd.clear();

    //освобождаем стек
    addedCmd["type"] = "cmd";
    addedCmd["args"] = input+output;
    addedCmd["cmd"] = "stack free";
    resultCmds.append(std::move(addedCmd));


}
}  // namespace syaz
