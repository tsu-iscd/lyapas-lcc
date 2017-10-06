#include "cmd_info.h"
#include <exception>
#include <tuple>
#include <vector>

namespace trm {

#define COMPARE(op) \
    size_t lsize = args.size(); \
    size_t rsize = rhs.args.size(); \
    \
    auto make = [](const CmdInfo &c, size_t &size) { \
        return std::tie(c.type, c.name, size); \
    }; \
    \
    return make(*this, lsize) op make(rhs, rsize)

bool CmdInfo::operator==(const CmdInfo &rhs) const
{
    COMPARE(==);
}

bool CmdInfo::operator<(const CmdInfo &rhs) const
{
    COMPARE(<);
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
