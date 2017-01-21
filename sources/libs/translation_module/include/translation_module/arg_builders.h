#pragma once

#include <string>
#include <memory>
#include "aliases.h"
#include "replacers.h"

namespace trm {

class ArgBuilder {
public:
    virtual ~ArgBuilder() = default;
    virtual Json::Value createArg() = 0;
};

class ConstArgBuilder : public ArgBuilder {
public:
    ConstArgBuilder(const Json::Value &sample) :
        sample(sample)
    {
    }

    Json::Value createArg() override
    {
        return sample;
    }

private:
    Json::Value sample;
};

class JsonArgBuilder : public ArgBuilder {
public:
    JsonArgBuilder(const std::string &fieldName, JsonMap &jsonMap) :
        fieldName(fieldName),
        jsonMap(jsonMap)
    {
    }

    Json::Value createArg() override
    {
        return jsonMap[fieldName];
    }

private:
    std::string fieldName;
    JsonMap &jsonMap;
};

class PatternArgBuilder : public ArgBuilder {
public:
    PatternArgBuilder(const std::string patternedString, const Replacers &replacers) :
        patternedString(patternedString),
        replacers(replacers)
    {
    }

    Json::Value createArg() override
    {
        throw std::runtime_error("Не реализовано.");
    }

    std::string patternedString;
    const Replacers &replacers;
};

std::shared_ptr<ArgBuilder> createArgBuilder(const std::string& value) {
    // arg:
    // int, "string" => ConstArgBuilder
    // {} => JsonArgBuilder
    // "<> <>" => PatternArgBuilder
}

}
