#include "variables_counter.h"
#include <regex>
#include <shared_utils/assertion.h>

namespace cyaz {

VariablesCounter::VariablesCounter()
{
    fullIgnore.insert("insert_string_in_complex");
    fullIgnore.insert("write_string");
    fullIgnore.insert("asm");

    firstArgIgnore.insert("definition");
    firstArgIgnore.insert("call");
}

void VariablesCounter::process(const Json::Value &program)
{
    for (const auto &cmd : program) {
        if (cmd["type"].asString() != "cmd") {
            return;
        }
        std::string cmdName = cmd["cmd"].asString();
        const Json::Value &args = cmd["args"];

        // переключатель между счетчиками
        if (cmdName == "definition") {
            LCC_ENSURE(args.size() >= 1, "args are too small");
            currentProcedure = args[0].asString();
            counters[currentProcedure] = 0;
        }

        if (fullIgnore.find(cmdName) != fullIgnore.end()) {
            continue;
        }

        unsigned index = 0;
        if (firstArgIgnore.find(cmdName) != firstArgIgnore.end()) {
            index = 1;
        }

        static const std::regex variable("t([0-9]+)");
        std::smatch match;
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
}

void VariablesCounter::updateState(const Json::Value &nextCmd)
{
    if (nextCmd["type"].asString() == "cmd" && nextCmd["cmd"].asString() == "definition") {
        const Json::Value &args = nextCmd["args"];
        LCC_ENSURE(args.size() >= 1, "args are too small");
        currentProcedure = args[0].asString();
    }
}

size_t VariablesCounter::getFree(size_t index)
{
    LCC_ASSERT(index >= 1);
    return counters[currentProcedure] + index;
}

}  // namespace cyaz
