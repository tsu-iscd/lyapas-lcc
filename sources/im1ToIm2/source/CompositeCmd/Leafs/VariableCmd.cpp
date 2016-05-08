#include "VariableCmd.h"//
// Created by s15-22 on 21.11.15.
//

#include "VariableCmd.h"

VariableCmd::VariableCmd(Json::Value json) : LeafCmd(json) { }

Json::Value VariableCmd::toJson()
{
    Json::Value result;
    result.clear();

    result["type"] = "v";
    result["args"] = _cmdJson["name"];

    return result;
}

std::string VariableCmd::toArgumentFormat() {
    return _cmdJson["name"].asString();
}
