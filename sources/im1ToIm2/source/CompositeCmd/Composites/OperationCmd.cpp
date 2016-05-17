//
// Created by s15-22 on 18.11.15.
//

#include "OperationCmd.h"

OperationCmd::OperationCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value OperationCmd::toJson()
{
    if(_children.size() == 0)
    {
        Json::Value result;
        result.clear();

        result["type"] = "cmd";
        result["cmd"] = _cmdJson["name"];

        return result;
    }
    else if(_children.size() == 1)
    {
        Json::Value result;
        result.clear();

        result["type"] = "cmd";

        auto operationName = _cmdJson["name"];
        auto child = _children.back()->toJson();
        result["cmd"] = operationName.asString() + std::string("_") + child["type"].asString();

        Json::Value childArgs = child["args"];
        if(childArgs.isArray())
        {
            result["args"] = childArgs;
        }
        else
        {
            Json::Value arrayWrapper;
            arrayWrapper.append(childArgs);
            result["args"] = arrayWrapper;
        }

        return result;
    }
    else
    {
        Json::Value result;
        result.clear();

        result["type"] = "cmd";
        result["cmd"] = _cmdJson["name"];

        Json::Value args;
        for(SPtr& child : _children)
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
