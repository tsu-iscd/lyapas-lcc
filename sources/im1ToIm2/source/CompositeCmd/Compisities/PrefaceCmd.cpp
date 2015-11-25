//
// Created by s15-22 on 25.11.15.
//

#include "PrefaceCmd.h"

PrefaceCmd::PrefaceCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value PrefaceCmd::toJson()
{
    Json::Value result;
    result.clear();


    for(SPtr& child : _clidren)
    {
        auto operation = child->toJson();
        result.append(operation);
    }

    return result;
}
