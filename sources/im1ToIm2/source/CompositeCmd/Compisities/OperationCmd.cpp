//
// Created by s15-22 on 18.11.15.
//

#include "OperationCmd.h"

OperationCmd::OperationCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value OperationCmd::toJson()
{
    if(_clidren.size() != 1) throw std::runtime_error("operation don't have one child");

    Json::Value result;
    result.clear();

    result["type"] = "cmd";

    auto operationName = _cmdJson["name"];
    auto child = _clidren.back()->toJson();
    result["cmd"] = operationName.asString() + std::string("_") + child["type"].asString();

    result["args"] = child["args"];

    return result;
}

