//
// Created by s15-22 on 18.11.15.
//

#include "OperationCmd.h"

OperationCmd::OperationCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value OperationCmd::toJson()
{
    if(_clidren.size() == 0)
    {
        Json::Value result;
        result.clear();

        result["type"] = "cmd";
        result["cmd"] = _cmdJson["name"];

        return result;
    }
    else if(_clidren.size() == 1)
    {
        Json::Value result;
        result.clear();

        result["type"] = "cmd";

        auto operationName = _cmdJson["name"];
        auto child = _clidren.back()->toJson();
        result["cmd"] = operationName.asString() + std::string("_") + child["type"].asString();

        Json::Value args;
        args.append(child["args"]);
        result["args"] = args;

        return result;
    }
    else
    {
        Json::Value result;
        result.clear();

        result["type"] = "cmd";
        result["cmd"] = _cmdJson["name"];

        Json::Value args;
        for(SPtr& child : _clidren)
        {
            for(auto& arg : child->toArgumentFormat())
            {
                args.append(arg);
            }
        }
        result["args"] = args;

        return result;
    }
}
