#pragma once

#include <string>
#include <unordered_map>
#include "icounter.h"

namespace cyaz {

class LabelsCounter : public ICounter {
public:
    void process(const Json::Value &program) override;
    void updateState(const Json::Value &nextCmd) override;
    size_t getFree(size_t index) override;

private:
    std::unordered_map<std::string, size_t> counters;
    std::string currentProcedure;
    size_t issuedCount = 0;
};

}  // namespace cyaz
