#pragma once

#include <memory>
#include <vector>
#include <json/json.h>
#include "aliases.h"
#include "cmd_info.h"
#include "translation_module.h"
#include "fillers.h"

namespace trm {

class CmdBuilder {
};
class ArgBuilder {
};

class CmdTranslator
{
public:
    CmdTranslator(const CmdInfo &src, const std::vector<CmdInfo> &dst)
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
