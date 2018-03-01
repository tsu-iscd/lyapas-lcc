#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "icounter.h"

namespace cyaz {

class VariablesCounter : public ICounter {
public:
    VariablesCounter();

    void process(const Json::Value &program) override;
    void updateState(const Json::Value &nextCmd) override;
    size_t getFree(size_t index) override;

private:
    std::unordered_set<std::string> fullIgnore;
    std::unordered_set<std::string> firstArgIgnore;

    std::unordered_map<std::string, size_t> counters;
    std::string currentProcedure;
};

}  // namespace cyaz
