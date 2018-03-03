#include "labels_counter.h"
#include <regex>
#include <shared_utils/assertion.h>

namespace cyaz {

void LabelsCounter::process(const Json::Value &program)
{
    for (const auto &cmd : program) {
        const std::string &type = cmd["type"].asString();

        if (type == "cmd" && cmd["cmd"].asString() == "definition") {
            const Json::Value &args = cmd["args"];
            LCC_ENSURE(args.size() >= 1, "args are too small");
            currentProcedure = args[0].asString();
            counters[currentProcedure] = 0;
        } else if (type == "label") {
            size_t labelNumber = cmd["number"].asUInt();
            counters[currentProcedure] = std::max(labelNumber, counters[currentProcedure]);
        }
    }
}

void LabelsCounter::updateState(const Json::Value &nextCmd)
{
    // все метки, которые были выданы во время
    // обработки предыдущей команды, считаются занятыми
    if (givenLabel) {
        counters[currentProcedure] += *givenLabel;
        givenLabel = trm::nullOpt;
    }

    if (nextCmd["type"].asString() == "cmd" && nextCmd["cmd"].asString() == "definition") {
        currentProcedure = nextCmd["args"][0].asString();
    }
}

size_t LabelsCounter::getFree(size_t index)
{
    LCC_ASSERT(index >= 1);
    givenLabel = givenLabel ? std::max(index, *givenLabel) : index;
    return counters[currentProcedure] + index;
}

}  // namespace cyaz
