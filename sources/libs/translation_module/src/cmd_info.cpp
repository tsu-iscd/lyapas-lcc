#include "cmd_info.h"
#include <exception>

namespace trm {

bool CmdInfo::operator==(const CmdInfo &rhs) const
{
    return rhs.type == type && rhs.name == name;
}

bool CmdInfo::operator<(const CmdInfo &rhs) const
{
    return rhs.type < type || (rhs.name < name);
}

CmdInfo createCmdInfo(const Json::Value &json)
{
    Json::Value type = json["type"];
    if (!type.isString()) {
        throw std::runtime_error("Object doesn`t have 'type' member or it isn`t string");
    }
    std::string typeStr = type.asString();

    if (typeStr == "cmd") {
        Json::Value cmd = json["cmd"];
        if (!cmd.isString()) {
            throw std::runtime_error("Cmd doesn`t have 'cmd' member or it isn`t string");
        }

        return {typeStr, cmd.asString()};
    } else if (typeStr == "lable") {
        return {typeStr, ""};
    } else {
        throw std::logic_error("Undefined type of object");
    }
}

}
