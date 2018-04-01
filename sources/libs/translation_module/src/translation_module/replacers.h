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
    PatternStringInfo(std::string pattern, const StringMap &stringMap)
        : stringMap(stringMap)
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
        return group ? ":" + std::to_string(*group) : "";
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

    // FIXME: вынести это поле из класса
    const StringMap &stringMap;
};

class Replacer {
public:
    virtual void updateState(const Json::Value &cmd) {}
    virtual std::string resolve(const PatternStringInfo &patternStringInfo) = 0;
};
using ReplacerPtr = std::shared_ptr<Replacer>;
using Replacers = std::map<std::string, ReplacerPtr>;

//
//  Вспомогательные макросы для заполнения Replacer'ов.
//
#define INSERT_REPLACER(replacers, name, type) (replacers).insert({name, std::make_shared<type>(cmds)})

#define INSERT_FUNCTIONAL_REPLACER(replacers, name, functionBlock)                                         \
    do {                                                                                                   \
        auto func = [&](const trm::PatternStringInfo &patternStringInfo) -> std::string { functionBlock }; \
        (replacers).insert({name, std::make_shared<cyaz::FunctionalReplacer>(func)});                      \
    } while (false)

}  // namespace trm
