//
// Created by vosafonov on 27.03.16.
//

#include "ArgsCmd.h"

ArgsCmd::ArgsCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value ArgsCmd::toJson()
{
    Json::Value result;
    result.clear();

    for(SPtr& child : _clidren)
    {
        for(auto& arg : child->toArgumentFormat())
        {
            result.append(arg);
        }
    }

    return result;
}
