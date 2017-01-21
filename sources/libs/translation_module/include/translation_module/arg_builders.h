#pragma once

#include <string>
#include <memory>
#include "aliases.h"
#include "replacers.h"
#include "cmd_translator_storage.h"

namespace trm {

class Packer {
public:
    virtual ~Packer() = default;
    virtual Json::Value pack(const std::string &value);
};
using PackerPtr = std::shared_ptr<Packer>;

class IntPacker : public Packer {
    Json::Value pack(const std::string &value) override
    {
        return Json::Value(std::stoll(value));
    }
};

class StringPacker : public Packer {
    Json::Value pack(const std::string &value) override
    {
        return Json::Value(value);
    }
};

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
    PatternArgBuilder(PackerPtr packer, const std::string patternedString, const Replacers &replacers) :
        packer(packer),
        patternedString(patternedString),
        replacers(replacers)
    {
    }

    Json::Value createArg() override
    {
        throw std::runtime_error("Не реализовано.");
    }

    PackerPtr packer;
    std::string patternedString;
    const Replacers &replacers;
};

std::shared_ptr<ArgBuilder> createArgBuilder(const std::string& value, CmdTranslatorStorage &storage) {
    // arg:
    // int, "string" => ConstArgBuilder
    // {} => JsonArgBuilder
    // "<> <>" => PatternArgBuilder
}

}
