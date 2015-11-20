//
// Created by Safonov Vadim on 11/4/15.
//

#include "ICmd.h"

ICmd::ICmd(Json::Value json) : _cmdJson(json[0]) {}

std::string ICmd::asString()
{
    return _cmdJson.toStyledString();
    //throw std::runtime_error("ICmd: can't use asString(), please override this function");
}

Json::Value ICmd::toJson()
{
    Json::Value cmd;
    cmd.clear();

    cmd["type"] = _cmdJson["type"].asString();
    cmd["cmd"] = "sample_cmd";

    Json::Value result;
    result.clear();

    result.append(cmd);

    return result;
}
