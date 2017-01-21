#pragma once

#include <functional>
#include <map>
#include <string>
#include "aliases.h"

namespace trm {

class PatternStringInfo {
    // name[:group][=param]
    PatternStringInfo(std::string pattern, const StringMap &stringMap) :
            stringMap(stringMap)
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
    }

    std::string name;
    Optional<unsigned long> group;
    Optional<std::string> param;

    const StringMap &stringMap;
};

using ReplaceFunction = std::function<std::string(const PatternStringInfo &patternStringInfo)>;
using Replacers = std::map<std::string, ReplaceFunction>;

}
