#include "free_label_replacer.h"
#include <stdexcept>
#include <translation_module/aliases.h>

namespace cyaz {

FreeLabelReplacer::FreeLabelReplacer(const Json::Value &program)
{
    counter.process(program);
}

void FreeLabelReplacer::updateState(const Json::Value &nextCmd)
{
    counter.updateState(nextCmd);
}

std::string FreeLabelReplacer::resolve(const trm::PatternStringInfo &patternStringInfo)
{
    trm::Optional<std::string> param = patternStringInfo.getParam();
    if (!param) {
        throw std::runtime_error("Паттерн " + patternStringInfo.getName() + " указан без параметра");
    }

    return std::to_string(counter.getFree(std::stoul(*param)));
}

}  // namespace cyaz
