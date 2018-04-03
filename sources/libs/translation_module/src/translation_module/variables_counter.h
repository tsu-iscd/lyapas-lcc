#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "icounter.h"

namespace trm {

//
// Класс используется для подсчёта переменных
// с именем "tN", где N число >= 0.
//
// Метод 'getFree' позволяет взять номер
// первой свободной переменной.
//
// Пример, где может понадобится подсчёт переменных:
// definition f1
// swap a, b
// swap t1, t2
//
// Для трансляции данного блока кода,
// нам понадобится свободная переменная.
// Т.к. t1 и t2 заняты, то будет использоваться
// следующая за ними свободная переменная t3.
//
// В результате получаем:
// definition f1
// move t3, a
// move b, t3
// move t3, t1
// move t2, t3
//
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

}  // namespace trm
