#pragma once

#include <memory>
#include <vector>
#include <json/json.h>

namespace trm {

class CmdBuilder {
public:
    CmdBuilder(Json::Value &&cmdSample, std::vector<std::shared_ptr<ArgBuilder>> &&argBuilders)
        : cmdSample(std::move(cmdSample))
        , argBuilders(std::move(argBuilders))
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

}  // namespace trm
