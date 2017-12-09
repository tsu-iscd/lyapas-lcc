#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <json/json.h>
#include "aliases.h"

namespace trm {

class PatternStringInfo {
public:
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
        nameWithGroup = name;
        if (group) {
            nameWithGroup = name + ':' + std::to_string(*group);
        }
    }

    const std::string &getName() const
    {
        return name;
    }

    const std::string &getNameWithGroup() const
    {
        return nameWithGroup;
    }

    const Optional<unsigned long> &getGroup() const
    {
        return group;
    }

    std::string getGroupAsString() const
    {
        if (group) {
            return ":" + std::to_string(*group);
        } else {
            return "";
        }
    }

    const Optional<std::string> &getParam() const
    {
        return param;
    }

    const StringMap &getStringMap() const
    {
        return stringMap;
    }

private:
    std::string name;
    std::string nameWithGroup;
    Optional<unsigned long> group;
    Optional<std::string> param;

    // FIXME(vsafonov): это поле тут не к месту
    const StringMap &stringMap;
};

class Replacer {
public:
    virtual void updateState(const Json::Value &cmd)
    {
    }
    virtual std::string resolve(const PatternStringInfo &patternStringInfo) = 0;
};
using ReplacerPtr = std::shared_ptr<Replacer>;
using Replacers = std::map<std::string, ReplacerPtr>;

}  // namespace trm
