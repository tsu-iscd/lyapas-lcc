#pragma once

#include <functional>
#include <map>
#include <string>
#include "aliases.h"

namespace trm {

class PatternStringInfo {
public:
    // name[:group][=param]
    PatternStringInfo(std::string pattern)
    {
        auto equals = pattern.rfind('=');
        if (equals != std::string::npos) {
            param = std::string(pattern.begin() + equals + 1, pattern.end());
            pattern = pattern.substr(0, equals);
        }

        auto colon = pattern.find(':');
        if (colon != std::string::npos) {
            group = std::stoul(std::string(pattern.begin() + colon + 1, pattern.end()));
            pattern = pattern.substr(0, colon);
        }

        name = std::move(pattern);
        nameWithGroup = name;
        if (group) {
            nameWithGroup = name + ':' + std::to_string(*group);
        }
    }

    const std::string &getName()
    {
        return name;
    }

    const std::string &getNameWithGroup()
    {
        return nameWithGroup;
    }

    const Optional<std::string> &getParam()
    {
        return param;
    }

private:
    std::string name;
    std::string nameWithGroup;
    Optional<unsigned long> group;
    Optional<std::string> param;
};

using ReplaceFunction = std::function<std::string(const PatternStringInfo &patternStringInfo)>;
using Replacers = std::map<std::string, ReplaceFunction>;

}  // namespace trm
