//
// Created by Safonov Vadim on 11/23/15.
//

#include "ComplexCmd.h"

ComplexCmd::ComplexCmd(Json::Value processJson) : CompositeCmd(processJson) { }

Json::Value ComplexCmd::toJson()
{
    if(_clidren.size() > 1) throw std::runtime_error("Complex can't have more one child; correct children = 1 or 0");

    Json::Value result;
    result.clear();

    if(_clidren.size() == 1)
    {
        auto child = _clidren.back()->toJson();

        result["type"] = _cmdJson["type"].asString() + "_" + _cmdJson["name"].asString() + "_" + child["type"].asString();
        result["args"] = child["args"];
    }
    else
    {
        result["type"] = _cmdJson["type"].asString() + "_" + _cmdJson["name"].asString();
        result["args"] = "";
    }

    return result;
}
