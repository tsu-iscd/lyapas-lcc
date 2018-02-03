#include "free_label_replacer.h"
#include <stdexcept>
#include <translation_module/aliases.h>

namespace cyaz {

FreeLabelReplacer::FreeLabelReplacer(const Json::Value &cmds)
{
    for (const auto &cmd : cmds) {
        counter.process(cmd);
    }
}

void FreeLabelReplacer::updateState(const Json::Value &cmd)
{
    counter.updateState(cmd);
}

std::string FreeLabelReplacer::resolve(const trm::PatternStringInfo &patternStringInfo)
{
    trm::Optional<std::string> param = patternStringInfo.getParam();
    if (!param) {
        throw std::runtime_error("Паттерн " + patternStringInfo.getName() + " указан без параметра");
    }

    return std::to_string(counter.getFreeLabel(std::stoul(*param)));
}

}  // namespace cyaz
