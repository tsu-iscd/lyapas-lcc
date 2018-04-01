#pragma once

#include <string>
#include <json/json.h>
#include "labels_counter.h"
#include "replacers.h"

namespace trm {

class FreeLabelReplacer : public Replacer {
public:
    explicit FreeLabelReplacer(const Json::Value &program);

    void updateState(const Json::Value &nextCmd) override;
    std::string resolve(const PatternStringInfo &patternStringInfo) override;

private:
    LabelsCounter counter;
};

}  // namespace trm
