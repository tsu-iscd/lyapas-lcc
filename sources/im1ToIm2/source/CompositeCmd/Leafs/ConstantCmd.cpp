//
// Created by s15-22 on 21.11.15.
//

#include "ConstantCmd.h"

ConstantCmd::ConstantCmd(Json::Value json) : LeafCmd(json) { }

Json::Value ConstantCmd::toJson()
{
    Json::Value result;

    result[fieldName::cmd_postfix] = "_c";
    result[fieldName::args].append(_cmdJson[fieldName::value]);

    return result;
}

Json::Value ConstantCmd::toArgumentFormat() {
    Json::Value value;
    value.append(_cmdJson[fieldName::value]);
    return value;
}
