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

        std::string complexType = _cmdJson["type"].asString();
        std::string complexNumber = _cmdJson["number"].asString();
        std::string childType = child["type"].asString();

        std::string type;
        type = complexType
            + (complexNumber.empty() ? "" : ("_" + complexNumber))
            + "_" + childType;

        std::string childArgs;
        childArgs = child["args"].asString();

        result["type"] = type;
        result["args"] = childArgs;
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
    if(_clidren.size() != 0) throw std::runtime_error("Complex can't have any children when it contains in arguments of procedure");
    Json::Value result;
    result.append(_cmdJson["type"].asString());
    result.append(_cmdJson["number"].asString());
    return result;
}
