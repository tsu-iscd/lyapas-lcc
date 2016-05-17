//
// Created by Safonov Vadim on 11/23/15.
//

#include "ComplexCmd.h"

ComplexCmd::ComplexCmd(Json::Value processJson) : CompositeCmd(processJson) { }

Json::Value ComplexCmd::toJson()
{
    if(_children.size() > 1) throw std::runtime_error("Complex can't have more one child; correct children = 1 or 0");

    Json::Value result;
    result.clear();

    if(_children.size() == 1)
    {
        auto child = _children.back()->toJson();

        result["type"] = child["type"];

        std::string complexType = _cmdJson["type"].asString();
        std::string complexNumber = _cmdJson["number"].asString();

        Json::Value args;
        args.append(complexType);
        if(!complexNumber.empty()) args.append(complexNumber);
        args.append(child["args"].asString());

        result["args"] = args;
    }
    else
    {
        std::string complexType = _cmdJson["type"].asString();
        std::string complexNumber = _cmdJson["number"].asString();

        std::string type;
        type = complexType;
        type += complexNumber.empty() ? "" : ("_" + complexNumber);

        result["type"] = type;
        result["args"] = "";
    }

    return result;
}

Json::Value ComplexCmd::toArgumentFormat() {
    if(_children.size() != 0) throw std::runtime_error("Complex can't have any children when it contains in arguments of procedure");
    Json::Value result;
    result.append(_cmdJson["type"].asString());
    result.append(_cmdJson["number"].asString());
    return result;
}
