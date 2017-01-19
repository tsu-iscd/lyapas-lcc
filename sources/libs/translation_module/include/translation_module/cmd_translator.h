#pragma once

#include <memory>
#include <vector>
#include <json/json.h>
#include "aliases.h"
#include "arg_builders.h"
#include "cmd_builder.h"
#include "cmd_info.h"
#include "translation_module.h"
#include "fillers.h"

namespace trm {

class CmdTranslator
{
public:
    CmdTranslator(const CmdInfo &src, const std::vector<CmdInfo> &dst)
    {
        for (const auto &arg : src.args) {
            srcFillers.push_back(createFiller(arg, srcArgJson, srcArgString));
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
                createArgBuilder(arg);
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

}
