//
// Created by s15-22 on 25.11.15.
//

#include "PrefaceCmd.h"

PrefaceCmd::PrefaceCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value PrefaceCmd::toJson()
{
    Json::Value result;

    for(SPtr& child : _children) {
        result.append(child->toJson());
    }

    return result;
}
