#pragma once

#include <set>
#include <string>
#include <cstdint>
#include <json/json.h>

namespace cyaz {

class LabelsCounter {
public:
    // используется для каждой команды из программы
    // перед началом трансляции для построения общей картины
    void process(const Json::Value &cmd);

    // вызывается перед трансляцией новой команды
    void updateState(const Json::Value &nextCmd);

    // используются во время трансляции команды
    size_t getFreeLabel(size_t index);

private:
    std::map<std::string, size_t> counters;
    std::string currentProcedure;
    size_t issuedCount = 0;
};

}  // namespace cyaz
