//
// Created by Safonov Vadim on 11/23/15.
//

#include "ComplexCmd.h"

ComplexCmd::ComplexCmd(Json::Value processJson) : CompositeCmd(processJson) { }

Json::Value ComplexCmd::toJson()
{
    if(_children.size() > 1) throw std::runtime_error("Complex can't have more one child; correct children = 1 or 0");

    Json::Value result;

    std::string complexType = _cmdJson[fieldName::type].asString();
    result[fieldName::args].append(complexType);

    if(_cmdJson.isMember(fieldName::number))
    {
        result[fieldName::args].append(_cmdJson[fieldName::number]);
    }

    if(_children.size() == 1)
    {
        auto child = _children.back()->toJson();
        for(auto arg : child[fieldName::args])
        {
            result[fieldName::args].append(arg);
        }

        result[fieldName::cmd_postfix] = child[fieldName::cmd_postfix];
    }
    else
    {
        result[fieldName::cmd_postfix] = "";
    }

    return result;
}

Json::Value ComplexCmd::toArgumentFormat() {
    if(_children.size() != 0) throw std::runtime_error("Complex can't have any children when it contains in arguments of procedure");

    Json::Value result;
    result.append(_cmdJson[fieldName::type].asString());
    result.append(_cmdJson[fieldName::number]);
    return result;
}
