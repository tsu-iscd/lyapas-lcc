#pragma once

#include <memory>
#include <vector>
#include <json/json.h>
#include "translation_module.h"
#include "cmd_info.h"

namespace trm {

using JsonMap = std::map<std::string, Json::Value>;
using StringMap = std::map<std::string, std::string>;
using IntMap = std::map<std::string, int64_t>;

//
// Fillers
//
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


class CmdBuilder {
};
class ArgBuilder {
};

class CmdTranslator
{
public:
    CmdTranslator(const std::vector<std::string> &args)
    {
    }

    std::vector<Json::Value> translate(const Json::Value &cmd)
    {
        if (cmd.size() != srcFillers.size()) {
            throw std::runtime_error("Cmd has invalid arguments count");
        }

        for (auto i = 0; i < cmd.size(); ++i) {
            srcFillers[i]->fill(cmd[i]);
        }

    }

private:
    JsonMap srcArgJson;
    StringMap srcArgString;
    IntMap srcArgInt;
    std::vector<std::unique_ptr<IFiller>> srcFillers;

    std::vector<CmdBuilder> cmdBuilders;

    //TODO(vsafonov): будут в cmd билдере
    //Json::Value cmdSample;
    //std::vector<std::unique_ptr<ArgBuilder>> argBuilders;
};

//
inline ArgBuilder createArgBuilder(const std::string& arg)
{
    return ArgBuilder();
}

}
