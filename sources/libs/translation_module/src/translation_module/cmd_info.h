#pragma once

#include <string>
#include <json/json.h>

namespace trm {

struct CmdInfo {
    std::string type;
    std::string name;
    std::vector<std::string> args;

    // TODO(vsafonov): может быть, они не нужны для map. нужно перепроверить
    bool operator==(const CmdInfo &rhs) const;
    bool operator<(const CmdInfo &rhs) const;
};

CmdInfo createCmdInfo(const Json::Value &json);

}  // namespace trm
