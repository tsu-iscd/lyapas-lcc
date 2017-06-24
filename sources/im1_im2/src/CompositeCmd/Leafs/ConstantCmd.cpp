#include "ConstantCmd.h"

ConstantCmd::ConstantCmd(Json::Value json) :
    LeafCmd(json)
{
}

Json::Value ConstantCmd::toJson()
{
    Json::Value result;

    result[fieldName::args].append(_cmdJson[fieldName::value]);

    return result;
}

Json::Value ConstantCmd::toArgumentFormat()
{
    Json::Value value;
    value.append(_cmdJson[fieldName::value]);
    return value;
}
