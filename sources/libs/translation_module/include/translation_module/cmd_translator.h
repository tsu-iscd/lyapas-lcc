#pragma once

#include <memory>
#include <vector>
#include <json/json.h>
#include "aliases.h"
#include "cmd_info.h"
#include "translation_module.h"
#include "fillers.h"

namespace trm {

class ArgBuilder {
public:
    Json::Value createArg();
};

class CmdBuilder {
public:
    CmdBuilder(Json::Value &&cmdSample,
               std::vector<std::shared_ptr<ArgBuilder>> &&argBuilders) :
        cmdSample(std::move(cmdSample)),
        argBuilders(std::move(argBuilders))
    {
    }

    Json::Value createCmd()
    {
        Json::Value result(cmdSample);

        if (!argBuilders.empty()) {
            Json::Value &args = result["args"] = Json::Value(Json::arrayValue);

            for (auto &argBuilder : argBuilders) {
                args.append(argBuilder->createArg());
            }
        }

        return result;
    }

private:
    Json::Value cmdSample;
    std::vector<std::shared_ptr<ArgBuilder>> argBuilders;
};

enum class ArgType {
    StringPattern,
    JsonPattern
};

ArgType cleanArg(std::string &arg) {
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

    if (isBorderedBy(arg, '{', '}')) {
        eraseBorders(arg);
        return ArgType::JsonPattern;
    }

    if (isBorderedBy(arg, '"', '"')) {
        eraseBorders(arg);
        if (isBorderedBy(arg, '<', '>')) {
            eraseBorders(arg);
            return ArgType::StringPattern;
        }
    }

    throw std::runtime_error("Некорректный тип аргумента");
}

class CmdTranslator
{
public:
    CmdTranslator(const CmdInfo &src, const std::vector<CmdInfo> &dst)
    {
        for (const auto &arg : src.args) {
            std::string cleanedArg(arg);
            ArgType argType = cleanArg(cleanedArg);
            switch (argType) {
            case ArgType::JsonPattern:
                // TODO(vsafonov): написать свою реализацию make_unique (либо перейти на C++14)
                srcFillers.push_back(std::unique_ptr<IFiller>(new JsonFiller(cleanedArg, srcArgJson)));
                break;
            case ArgType::StringPattern:
                srcFillers.push_back(std::unique_ptr<IFiller>(new StringFiller(cleanedArg, srcArgString)));
                break;
            default:
                throw std::runtime_error("Обработчик не установлен");
            }
        }

        for (const auto &dstCmd : dst) {
            Json::Value sample;
            if (dstCmd.type.empty()) {
                throw std::runtime_error("Некорректный тип");
            }
            sample["type"] = dstCmd.type;
            sample["name"] = dstCmd.name;

            std::vector<std::shared_ptr<ArgBuilder>> argBuilders;
            for (const auto &arg : dstCmd.args) {
                // arg:
                // int, "string" => ConstArgBuilder
                // {} => JsonArgBuilder
                // "<> <>" => PatternArgBuilder
            }

            cmdBuilders.emplace_back(std::move(sample), std::move(argBuilders));
        }
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
    std::vector<std::unique_ptr<IFiller>> srcFillers;
    std::vector<CmdBuilder> cmdBuilders;

    JsonMap srcArgJson;
    StringMap srcArgString;
    IntMap srcArgInt;
};

//
inline ArgBuilder createArgBuilder(const std::string& arg)
{
    return ArgBuilder();
}

}
