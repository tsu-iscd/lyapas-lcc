#pragma once

#include <vector>
#include <json/json.h>
#include "translation_module.h"

namespace trm {

class IFiller {
};
class ArgBuilder {
};

class CmdTranslator
{
public:
    CmdTranslator(std::vector<std::vector<ArgBuilder>> &&argBuilders, std::vector<IFiller> &&fillers) :
        argBuilders(std::move(argBuilders)),
        fillers(std::move(fillers))
    {
    }

    std::vector<Json::Value> translate(const Json::Value &cmd)
    {
        return {};
    }

private:
    std::vector<std::vector<ArgBuilder>> argBuilders;
    std::vector<IFiller> fillers;
};

//
inline ArgBuilder createArgBuilder(const std::string& arg)
{
    return ArgBuilder();
}

}
