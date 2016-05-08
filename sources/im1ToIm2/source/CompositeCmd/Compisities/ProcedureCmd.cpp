//
// Created by Safonov Vadim on 11/6/15.
//

#include <iostream>
#include "ProcedureCmd.h"

ProcedureCmd::ProcedureCmd(Json::Value processJson, ProcedureCmd::Type type)
        : CompositeCmd(processJson), _type(type) {}

Json::Value ProcedureCmd::toJson()
{
    auto signature = buildSignature();

    if(_type == ProcedureCmd::Type::call)
    {
        return signature;
    }
    else if(_type == ProcedureCmd::Type::definition)
    {
        Json::Value result;
        result.clear();

        result.append(signature);
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
}

Json::Value ProcedureCmd::buildSignature()
{
    Json::Value signature;
    signature["type"] = _cmdJson["type"].asString();
    signature["args"] = Json::Value();
    auto& signatureArgs = signature["args"];
    signatureArgs.clear();

    signatureArgs.append(_cmdJson["name"].asString());

    auto inArgs = _clidren.front()->toJson();
    for(auto& arg : inArgs)
    {
        signatureArgs.append(arg);
    }
    _clidren.pop_front();

    signatureArgs.append("/");

    auto outArgs = _clidren.front()->toJson();
    for(auto& arg : outArgs)
    {
        signatureArgs.append(arg);
    }
    _clidren.pop_front();

    return signature;
}


