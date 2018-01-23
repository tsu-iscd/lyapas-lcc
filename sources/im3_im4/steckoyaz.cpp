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
            resultCmds.append(std::move(cmd));
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
}
}  // namespace syaz
