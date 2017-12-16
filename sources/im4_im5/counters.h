#pragma once

#include <regex>
#include <set>
#include <string>
#include <json/json.h>
#include <shared_utils/assertion.h>

namespace cyaz {

class VariableCounter {
public:
    VariableCounter()
    {
        fullIgnore.insert("insert_string_in_complex");
        fullIgnore.insert("write_string");
        fullIgnore.insert("asm");

        firstArgIgnore.insert("definition");
        firstArgIgnore.insert("call");
    }

    void process(const JSON &cmd)
    {
        if (cmd["type"].asString() != "cmd") {
            return;
        }
        std::string cmdName = cmd["cmd"].asString();

        // переключатель между счетчиками
        if (cmdName == "definition") {
            currentProcedure = cmd["args"][0].asString();
            counters[currentProcedure] = 0;
        }

        if (fullIgnore.find(cmdName) != fullIgnore.end()) {
            return;
        }

        unsigned index = 0;
        if (firstArgIgnore.find(cmdName) != firstArgIgnore.end()) {
            index = 1;
        }

        static const std::regex variable("t([0-9]+)");
        std::smatch match;
        const auto &args = cmd["args"];
        for (; index < args.size(); ++index) {
            const auto &arg = args[index];
            if (arg.isString()) {
                std::string argStr(arg.asString());
                if (std::regex_match(argStr, match, variable) && match.size() == 2) {
                    counters[currentProcedure] = std::max(std::stoul(match[1].str()), counters[currentProcedure]);
                }
            }
        }
    }

    void updateState(const JSON &nextCmd)
    {
        if (nextCmd["type"].asString() == "cmd" && nextCmd["cmd"].asString() == "definition") {
            currentProcedure = nextCmd["args"][0].asString();
        }
    }

    unsigned long getFreeVariable(unsigned long index)
    {
        return counters[currentProcedure] + index;
    }

private:
    std::set<std::string> fullIgnore;
    std::set<std::string> firstArgIgnore;

    std::map<std::string, unsigned long> counters;
    std::string currentProcedure;
};

class LabelCounter {
public:
    // используется перед трансляцией программы для построения общей картины
    void process(const JSON &cmd)
    {
        const std::string &cmdType = cmd["type"].asString();

        // переключение между счетчиками
        if (cmdType == "cmd" && cmd["cmd"].asString() == "definition") {
            const Json::Value &args = cmd["args"];
            LCC_ENSURE(args.size() >= 1, "cmd args is too small");
            currentProcedure = args[0].asString();
            counters[currentProcedure] = 0;
        } else if (cmdType == "label") {
            counters[currentProcedure] = std::max(cmd["number"].asUInt(), counters[currentProcedure]);
        }
    }

    // вызывается перед трансляцией новой команды
    void updateState(const JSON &nextCmd)
    {
        if (issuedCount) {
            counters[currentProcedure] += issuedCount;
            issuedCount = 0;
        }

        if (nextCmd["type"].asString() == "cmd" && nextCmd["cmd"].asString() == "definition") {
            currentProcedure = nextCmd["args"][0].asString();
        }
    }

    // используются во время трансляции команды
    unsigned getFreeLabel(unsigned index)
    {
        issuedCount = std::max(index, issuedCount);
        return counters[currentProcedure] + index;
    }

private:
    std::map<std::string, unsigned> counters;
    std::string currentProcedure;
    unsigned issuedCount = 0;
};

}  // namespace cyaz