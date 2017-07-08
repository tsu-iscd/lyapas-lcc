#include "StringCmd.h"

StringCmd::StringCmd(Json::Value json) :
    LeafCmd(json)
{
}

Json::Value StringCmd::toJson()
{
    Json::Value result;

    result[fieldName::args].append("\"" + _cmdJson[fieldName::value].asString() + "\"");

    return result;
}

Json::Value StringCmd::toArgumentFormat()
{
    Json::Value value;
    value.append("\"" + _cmdJson[fieldName::value].asString() + "\"");
    return value;
}
