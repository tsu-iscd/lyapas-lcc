#pragma once

#include <cstdint>
#include <json/json.h>

namespace trm {

class ICounter {
public:
    // перед использованием класса, требуется вызвать этот метод,
    // передав обрабатываемую программу
    virtual void process(const Json::Value &program) = 0;

    // метод вызывается перед трансляцией очередной команды
    virtual void updateState(const Json::Value &nextCmd) = 0;

    // метод используются во время трансляции команды
    virtual size_t getFree(size_t index) = 0;
};

}  // namespace trm
