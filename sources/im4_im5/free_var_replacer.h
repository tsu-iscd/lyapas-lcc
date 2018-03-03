#pragma once

#include <string>
#include <json/json.h>
#include <translation_module/replacers.h>
#include "variables_counter.h"

namespace cyaz {

class FreeVarReplacer : public trm::Replacer {
public:
    explicit FreeVarReplacer(const Json::Value &program);

    void updateState(const Json::Value &nextCmd) override;
    std::string resolve(const trm::PatternStringInfo &patternStringInfo) override;

private:
    VariablesCounter counter;
};

}  // namespace cyaz