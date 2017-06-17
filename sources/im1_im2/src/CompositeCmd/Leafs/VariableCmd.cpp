#include "VariableCmd.h"

VariableCmd::VariableCmd(Json::Value json) :
    LeafCmd(json)
{
}

Json::Value VariableCmd::toJson()
{
    Json::Value result;

    result[fieldName::cmd_postfix] = "_v";
    result[fieldName::args].append(_cmdJson[fieldName::name]);

    return result;
}

Json::Value VariableCmd::toArgumentFormat()
{
    Json::Value value;
    value.append(_cmdJson[fieldName::name]);
    return value;
}
