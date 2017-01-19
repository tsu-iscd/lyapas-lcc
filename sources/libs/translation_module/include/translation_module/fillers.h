#pragma once

#include <memory>
#include <json/json.h>
#include "aliases.h"

namespace trm {

class IFiller {
public:
    virtual void fill(const Json::Value &json) = 0;
};

class JsonFiller : public IFiller {
public:
    JsonFiller(const std::string &fieldName, JsonMap &jsonMap) :
        fieldName(fieldName),
        jsonMap(jsonMap)
    {
    }

    void fill(const Json::Value &json) override
    {
        jsonMap[fieldName] = json;
    }
private:
    std::string fieldName;
    JsonMap &jsonMap;
};

class StringFiller : public IFiller {
public:
    StringFiller(const std::string &fieldName, StringMap &stringMap) :
        fieldName(fieldName),
        stringMap(stringMap)
    {
    }

    void fill(const Json::Value &json) override
    {
        if (!json.isString()) {
            throw std::runtime_error("Ожидался JSON с типом String.");
        }
        stringMap[fieldName] = json.asString();
    }
private:
    std::string fieldName;
    StringMap &stringMap;
};


std::unique_ptr<IFiller> createFiller(std::string value, JsonMap &jsonMap, StringMap &stringMap) {
    auto eraseBorders = [](std::string &str) {
        if (str.size() >= 2) {
            str = std::string(str.begin() + 1, str.end() - 1);
        } else {
            throw std::runtime_error("string is too small");
        }
    };

    auto isBorderedBy = [](std::string &str, char b, char e) {
        if (str.size() < 2) {
            return false;
        }
        return str.front() == b && str.back() == e;
    };

    if (isBorderedBy(value, '{', '}')) {
        eraseBorders(value);
        return std::unique_ptr<IFiller>(new JsonFiller(value, jsonMap));
    }

    if (isBorderedBy(value, '"', '"')) {
        eraseBorders(value);
        if (isBorderedBy(value, '<', '>')) {
            eraseBorders(value);
            return std::unique_ptr<IFiller>(new StringFiller(value, stringMap));
        }
    }

    throw std::runtime_error("Некорректное значение строки");
}

}
