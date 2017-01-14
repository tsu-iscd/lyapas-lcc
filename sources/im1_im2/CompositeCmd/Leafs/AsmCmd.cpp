//
// Created by vosafonov on 16.05.16.
//

#include "AsmCmd.h"

AsmCmd::AsmCmd(Json::Value json) : LeafCmd(json) { }

Json::Value AsmCmd::toJson()
{
    Json::Value result;

    result[fieldName::type] = "cmd";
    result[fieldName::cmd] = "asm";
    result[fieldName::args].append(_cmdJson[fieldName::command]);

    return result;
}
