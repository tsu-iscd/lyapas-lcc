#include "ArgsCmd.h"

ArgsCmd::ArgsCmd(Json::Value processJson)
    : CompositeCmd(processJson)
{
}

Json::Value ArgsCmd::toJson()
{
    Json::Value result;

    for (SPtr &child : _children) {
        for (auto &arg : child->toArgumentFormat()) {
            result.append(arg);
        }
    }

    return result;
}
