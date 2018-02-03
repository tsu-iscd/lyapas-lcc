#pragma once

#include <string>
#include <json/json.h>
#include <translation_module/replacers.h>
#include "labels_counter.h"

namespace cyaz {

class FreeLabelReplacer : public trm::Replacer {
public:
    FreeLabelReplacer(const Json::Value &cmds);

    void updateState(const Json::Value &cmd) override;
    std::string resolve(const trm::PatternStringInfo &patternStringInfo) override;

private:
    LabelsCounter counter;
};

}  // namespace cyaz
