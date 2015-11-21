//
// Created by s15-22 on 21.11.15.
//

#include "ConstantCmd.h"

ConstantCmd::ConstantCmd(Json::Value json) : LeafCmd(json) { }

Json::Value ConstantCmd::toJson()
{
    Json::Value result;
    result.clear();

    result["type"] = "c";
    result["args"] = _cmdJson["name"];

    return result;
}
