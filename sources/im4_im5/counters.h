#pragma once

#include <string>
#include <set>
#include <regex>
#include <json/json.h>

namespace cyaz {

class VariableCounter {
public:
    VariableCounter()
    {
        fullIgnore.insert("insert_string_in_complex");
        fullIgnore.insert("write_string");
        fullIgnore.insert("asm");

        firstArgIgnore.insert("definition_procedure");
        firstArgIgnore.insert("call");
    }

    void process(const JSON &cmd)
    {
        if (cmd["type"].asString() != "cmd") {
            return;
        }
        std::string cmdName = cmd["cmd"].asString();

        // переключатель между счетчиками
        if (cmdName == "definition_procedure")
        {
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

        static const std::regex variable("t?([0-9]+)");
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
        if (nextCmd["type"].asString() == "cmd") {
            if (nextCmd["cmd"].asString() == "definition_procedure") {
                currentProcedure = nextCmd["args"][0].asString();
                return;
            }
        }

        if (issuedCount) {
            counters[currentProcedure] += issuedCount;
            issuedCount = 0;
        }
    }

    unsigned long getFreeVariable(unsigned long index)
    {
        issuedCount = std::max(index, issuedCount);
        return counters[currentProcedure] + index;
    }

private:
    std::set<std::string> fullIgnore;
    std::set<std::string> firstArgIgnore;

    std::map<std::string, unsigned long> counters;
    std::string currentProcedure;
    unsigned long issuedCount = 0;
};

class LabelCounter {
public:
    // используется перед трансляцией программы для построения общей картины
    void process(const JSON &cmd)
    {
        // переключатель между счетчиками
        if (cmd["type"].asString() == "cmd") {
            if (cmd["cmd"].asString() == "definition_procedure")
            {
                currentProcedure = cmd["args"][0].asString();
                counters[currentProcedure] = 0;
            }
        }


        if (cmd["type"].asString() == "label") {
            counters[currentProcedure] = std::max(cmd["number"].asUInt(), counters[currentProcedure]);
        }
    }

    // вызывается перед трансляцией новой команды
    void updateState(const JSON &nextCmd)
    {
        if (nextCmd["type"].asString() == "cmd") {
            if (nextCmd["cmd"].asString() == "definition_procedure")
            {
                currentProcedure = nextCmd["args"][0].asString();
                return;
            }
        }

        if (issuedCount) {
            counters[currentProcedure] += issuedCount;
            issuedCount = 0;
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

}


