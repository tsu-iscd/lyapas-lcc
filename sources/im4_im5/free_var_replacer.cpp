#include "free_var_replacer.h"
#include <stdexcept>
#include <translation_module/aliases.h>

namespace cyaz {

FreeVarReplacer::FreeVarReplacer(const Json::Value &cmds)
{
    for (const auto &cmd : cmds) {
        counter.process(cmd);
    }
}

void FreeVarReplacer::updateState(const Json::Value &cmd)
{
    counter.updateState(cmd);
}

std::string FreeVarReplacer::resolve(const trm::PatternStringInfo &patternStringInfo)
{
    trm::Optional<std::string> param = patternStringInfo.getParam();
    if (!param) {
        throw std::runtime_error("Паттерн " + patternStringInfo.getName() + " указан без параметра");
    }

    return 't' + std::to_string(counter.getFree(std::stoul(*param)));
}

}  // namespace cyaz
