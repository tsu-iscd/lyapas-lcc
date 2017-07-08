#include "ICmd.h"

ICmd::ICmd(Json::Value json) :
    _cmdJson(json[0])
{
}

std::string ICmd::asString()
{
    return _cmdJson.toStyledString();
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

Json::Value ICmd::toArgumentFormat()
{
    throw std::runtime_error(std::string(typeid(*this).name())
                             + "ICmd: can't use toArgumentFormat(), please override this function\n"
                             + _cmdJson.toStyledString());
}
