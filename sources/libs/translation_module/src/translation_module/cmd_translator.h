#pragma once

#include <memory>
#include <vector>
#include <json/json.h>
#include <shared_utils/assertion.h>
#include "aliases.h"
#include "arg_builders.h"
#include "cmd_builder.h"
#include "cmd_info.h"
#include "cmd_translator_storage.h"
#include "fillers.h"
#include "translation_module.h"

namespace trm {

class CmdTranslator {
public:
    CmdTranslator(const CmdInfo &src, const std::vector<CmdInfo> &dst, const Replacers &replacers)
        : storage{replacers, {}, {}, {}}
    {
        for (const auto &arg : src.args) {
            srcFillers.push_back(createFiller(arg, storage));
        }

        for (const auto &dstCmd : dst) {
            Json::Value sample;
            if (dstCmd.type.empty()) {
                throw std::runtime_error("Некорректный тип");
            }
            sample["type"] = dstCmd.type;
            sample["cmd"] = dstCmd.name;

            std::vector<std::shared_ptr<ArgBuilder>> argBuilders;
            for (const auto &arg : dstCmd.args) {
                argBuilders.push_back(createArgBuilder(arg, storage));
            }

            cmdBuilders.emplace_back(std::move(sample), std::move(argBuilders));
        }
    }

    CmdTranslator(const CmdTranslator &other) = delete;

    std::vector<Json::Value> translate(const Json::Value &cmd)
    {
        const Json::Value &args = cmd["args"];

        LCC_ASSERT(args.size() == srcFillers.size());
        for (auto i = 0; i < args.size(); ++i) {
            srcFillers[i]->fill(args[i]);
        }

        std::vector<Json::Value> result;
        for (auto &cmdBuilder : cmdBuilders) {
            result.push_back(cmdBuilder.createCmd());
        }
        return result;
    }

private:
    std::vector<std::unique_ptr<IFiller>> srcFillers;
    std::vector<CmdBuilder> cmdBuilders;

    CmdTranslatorStorage storage;
};

}  // namespace trm
