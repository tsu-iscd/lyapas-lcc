#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <json/json.h>

namespace cyaz {

class VariablesCounter {
public:
    VariablesCounter();

    // используется для каждой команды из программы
    // перед началом трансляции для построения общей картины
    void process(const Json::Value &cmd);

    // вызывается перед трансляцией новой команды
    void updateState(const Json::Value &nextCmd);

    // используются во время трансляции команды
    size_t getFreeVariable(size_t index);

private:
    std::set<std::string> fullIgnore;
    std::set<std::string> firstArgIgnore;

    std::map<std::string, size_t> counters;
    std::string currentProcedure;
};

}
