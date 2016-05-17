//
// Created by vosafonov on 15.05.16.
//

#include "StringCmd.h"

StringCmd::StringCmd(Json::Value json) : LeafCmd(json) { }

Json::Value StringCmd::toJson()
{
    Json::Value result;
    result.clear();

    result["type"] = "s";
    result["args"] = "\"" + _cmdJson["value"].asString() + "\"";

    return result;
}

//this function doesn't look good.
//we shoult think about it
Json::Value StringCmd::toArgumentFormat() {
    Json::Value value;
    value.append("\"" + _cmdJson["value"].asString() + "\"");
    return value;
}
