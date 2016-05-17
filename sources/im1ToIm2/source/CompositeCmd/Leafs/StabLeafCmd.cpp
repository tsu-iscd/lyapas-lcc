//
// Created by Safonov Vadim on 11/6/15.
//

#include "StabLeafCmd.h"

StabLeafCmd::StabLeafCmd(Json::Value json) : LeafCmd(json) { }

Json::Value StabLeafCmd::toJson()
{
    Json::Value result;

    result["type"] = "!!stab!!";
    result["args"] = "!!stab!!";

    return result;
}
