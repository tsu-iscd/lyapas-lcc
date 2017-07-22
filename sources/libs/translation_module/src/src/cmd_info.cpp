#include "cmd_info.h"
#include <exception>
#include <vector>

namespace trm {

bool CmdInfo::operator==(const CmdInfo &rhs) const
{
    return type == rhs.type &&
       name == rhs.name &&
       args.size() == rhs.args.size();
}

bool CmdInfo::operator<(const CmdInfo &rhs) const
{
    if (type != rhs.type) {
        return type < rhs.type;
    }

    if (name != rhs.name) {
        return name < rhs.name;
    }

    if (args.size() != rhs.args.size()) {
        return args.size() < rhs.args.size();
    }

    return true;
}

CmdInfo createCmdInfo(const Json::Value &json)
{
    std::vector<std::string> args;
    for (const auto &arg : json["args"]) {
        args.push_back(arg.asString());
    }

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

        return {typeStr, cmd.asString(), std::move(args)};
    } else if (typeStr == "label" || typeStr == "definition") {
        return {typeStr, "", std::move(args)};
    } else {
        throw std::logic_error("Undefined type of object: '" + typeStr + "'");
    }
}

}
