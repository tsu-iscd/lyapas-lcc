#include "function_info.h"

FunctionInfo::FunctionInfo(const JSON &cmd)
{
    //считаем количество входных/выходных параметров
    auto i = cmd["args"].begin();
    name = (*i).asString();
    i++;

    for (i; (*i) != "/"; i++) {
        input.push_back((*i));
    }

    //пропускаем "/"
    for (i++; i != cmd["args"].end(); i++) {
        output.insert(output.end(), (*i));
    }
}