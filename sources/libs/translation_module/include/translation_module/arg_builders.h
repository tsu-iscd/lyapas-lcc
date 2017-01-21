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
    virtual Json::Value pack(const std::string &value) = 0;
};
using PackerPtr = std::shared_ptr<Packer>;

class IntPacker : public Packer {
public:
    Json::Value pack(const std::string &value) override
    {
        return Json::Value(std::stoll(value));
    }
};

class StringPacker : public Packer {
public:
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
    PatternArgBuilder(PackerPtr packer, const std::string patternedString, const CmdTranslatorStorage &storage) :
        packer(packer),
        patternedString(patternedString),
        storage(storage)
    {
    }

    Json::Value createArg() override
    {
        throw std::runtime_error("Не реализовано.");
    }

    PackerPtr packer;
    std::string patternedString;
    const CmdTranslatorStorage &storage;
};

std::shared_ptr<ArgBuilder> createArgBuilder(std::string value, CmdTranslatorStorage &storage) {
    //FIXME(vsafonov): DRY, эти функции повторяеются в fillers.h
    auto eraseBorders = [](std::string &str) {
        if (str.size() >= 2) {
            str = std::string(str.begin() + 1, str.end() - 1);
        } else {
            throw std::runtime_error("string is too small");
        }
    };

    auto isBorderedBy = [](const std::string &str, char b, char e) {
        if (str.size() < 2) {
            return false;
        }
        return str.front() == b && str.back() == e;
    };

    // {} => JsonArgBuilder
    if (isBorderedBy(value, '{', '}')) {
        eraseBorders(value);
        return std::make_shared<JsonArgBuilder>(value, storage.srcArgJson);
    }

    // int, "string" => ConstArgBuilder
    // <> <>, "<> <>" => PatternArgBuilder
    PackerPtr packer;
    if (isBorderedBy(value, '"', '"')) {
        eraseBorders(value);
        packer = std::make_shared<StringPacker>();
    } else {
        packer = std::make_shared<IntPacker>();
    }

    auto left = value.find('<');
    auto right = value.rfind('>');
    if (left != std::string::npos && right != std::string::npos && left < right) {
        return std::make_shared<PatternArgBuilder>(packer, value, storage);
    } else {
        return std::make_shared<ConstArgBuilder>(packer->pack(value));
    }
}

}
