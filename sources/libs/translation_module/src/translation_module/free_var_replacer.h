#pragma once

#include <string>
#include <json/json.h>
#include "replacers.h"
#include "variables_counter.h"

namespace trm {

class FreeVarReplacer : public Replacer {
public:
    explicit FreeVarReplacer(const Json::Value &program);

    void updateState(const Json::Value &nextCmd) override;
    std::string resolve(const PatternStringInfo &patternStringInfo) override;

private:
    VariablesCounter counter;
};

}  // namespace trm
