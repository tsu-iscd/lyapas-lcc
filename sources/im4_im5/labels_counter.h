#pragma once

#include <string>
#include <unordered_map>
#include <translation_module/aliases.h>
#include "icounter.h"

namespace cyaz {

//
// Класс используется для подсчёта меток.
//
// Метод 'getFree' позволяет взять номер
// первой свободной метки.
//
// Пример, где может понадобится подсчёт меток:
// definition f1
// label 1:
// clear_first_and_second F1
//
// Для трансляции данного блока кода,
// нам понадобятся две свободные метки.
// Т.к. метка 1 занята, то будет использоваться
// следующая за ней свободная метка 2.
//
// В результате получаем:
// definition f1
// label 1:
// move t1, 0
// label 2:
// move F1[t1], 0
// inc t1
// cmp t1, 2
// jne 2
//
class LabelsCounter : public ICounter {
public:
    void process(const Json::Value &program) override;
    void updateState(const Json::Value &nextCmd) override;
    size_t getFree(size_t index) override;

private:
    std::unordered_map<std::string, size_t> counters;
    std::string currentProcedure;
    trm::Optional<size_t> givenLabel;
};

}  // namespace cyaz
