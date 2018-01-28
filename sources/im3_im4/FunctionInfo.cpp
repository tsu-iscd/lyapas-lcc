#include "FunctionInfo.h"

FunctionInfo::FunctionInfo(const JSON &cmd)
{
    //считаем количество входных/выходных параметров
    auto i = cmd["args"].begin();
    functionName = (*i).asString();
    i++;

    for (i; (*i) != "/"; i++) {
        input.insert(input.end(), (*i));
    }

    //пропускаем "/"
    for (i++; i != cmd["args"].end(); i++) {
        output.insert(output.end(), (*i));
    }
}