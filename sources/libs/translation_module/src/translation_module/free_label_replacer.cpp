#include "free_label_replacer.h"
#include <stdexcept>
#include "aliases.h"

namespace trm {

FreeLabelReplacer::FreeLabelReplacer(const Json::Value &program)
{
    counter.process(program);
}

void FreeLabelReplacer::updateState(const Json::Value &nextCmd)
{
    counter.updateState(nextCmd);
}

std::string FreeLabelReplacer::resolve(const PatternStringInfo &patternStringInfo)
{
    Optional<std::string> param = patternStringInfo.getParam();
    if (!param) {
        throw std::runtime_error("Паттерн " + patternStringInfo.getName() + " указан без параметра");
    }

    return std::to_string(counter.getFree(std::stoul(*param)));
}

}  // namespace trm
