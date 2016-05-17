//
// Created by s15-22 on 18.11.15.
//

#include "OperationCmd.h"

OperationCmd::OperationCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value OperationCmd::toJson()
{
    Json::Value result;
    result["type"] = "cmd";
    auto operationName = _cmdJson["name"];

    if(_children.size() == 0)
    {
        result["cmd"] = operationName;
    }
    else if(_children.size() == 1)
    {
        auto child = _children.back()->toJson();
        result[fieldName::args] = child[fieldName::args];

        result["cmd"] = operationName.asString() + child[fieldName::cmd_postfix].asString();
    }
    else
    {
        result["cmd"] = operationName;

        for(SPtr& child : _children)
        {
            for(auto& arg : child->toArgumentFormat())
            {
                result[fieldName::args].append(arg);
            }
        }
    }

    return result;
}
