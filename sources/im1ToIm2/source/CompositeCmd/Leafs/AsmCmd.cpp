//
// Created by vosafonov on 16.05.16.
//

#include "AsmCmd.h"

AsmCmd::AsmCmd(Json::Value json) : LeafCmd(json) { }

Json::Value AsmCmd::toJson()
{
    Json::Value result;
    result.clear();

    result["type"] = "cmd";
    result["cmd"] = "asm";

    Json::Value args;
    args.append(_cmdJson["command"]);
    result["args"] = args;

    return result;
}
