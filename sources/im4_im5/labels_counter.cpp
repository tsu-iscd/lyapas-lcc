#include "labels_counter.h"
#include <regex>
#include <shared_utils/assertion.h>

namespace cyaz {

void LabelsCounter::process(const Json::Value &program)
{
    for (const auto &cmd : program) {
        const std::string &cmdType = cmd["type"].asString();

        // переключение между счетчиками
        if (cmdType == "cmd" && cmd["cmd"].asString() == "definition") {
            const Json::Value &args = cmd["args"];
            LCC_ENSURE(args.size() >= 1, "args are too small");
            currentProcedure = args[0].asString();
            counters[currentProcedure] = 0;
        } else if (cmdType == "label") {
            size_t labelNumber = cmd["number"].asUInt();
            counters[currentProcedure] = std::max(labelNumber, counters[currentProcedure]);
        }
    }
}

void LabelsCounter::updateState(const Json::Value &nextCmd)
{
    if (issuedCount) {
        counters[currentProcedure] += issuedCount;
        issuedCount = 0;
    }

    if (nextCmd["type"].asString() == "cmd" && nextCmd["cmd"].asString() == "definition") {
        currentProcedure = nextCmd["args"][0].asString();
    }
}

size_t LabelsCounter::getFree(size_t index)
{
    issuedCount = std::max(index, issuedCount);
    return counters[currentProcedure] + index;
}

}  // namespace cyaz
