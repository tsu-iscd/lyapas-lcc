#pragma once

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

}
