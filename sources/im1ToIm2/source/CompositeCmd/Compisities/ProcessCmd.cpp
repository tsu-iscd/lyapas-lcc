//
// Created by Safonov Vadim on 11/6/15.
//

#include "ProcessCmd.h"

ProcessCmd::ProcessCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value ProcessCmd::toJson()
{
    Json::Value result;
    result.clear();

    for(SPtr& child : _clidren)
    {
        auto paragraph = child->toJson();
        for(auto cmd : paragraph)
        {
            result.append(cmd);
        }
    }

    return result;
}
