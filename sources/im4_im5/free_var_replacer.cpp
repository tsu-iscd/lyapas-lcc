#include "free_var_replacer.h"
#include <stdexcept>
#include <translation_module/aliases.h>

namespace cyaz {

FreeVarReplacer::FreeVarReplacer(const Json::Value &program)
{
    counter.process(program);
}

void FreeVarReplacer::updateState(const Json::Value &nextCmd)
{
    counter.updateState(nextCmd);
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
