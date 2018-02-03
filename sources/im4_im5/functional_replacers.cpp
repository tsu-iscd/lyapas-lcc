#include "functional_replacers.h"

namespace cyaz {

FunctionalReplacer::FunctionalReplacer(ReplaceFunction function)
    : function(std::move(function))
{
}

std::string FunctionalReplacer::resolve(const trm::PatternStringInfo &patternStringInfo)
{
    return function(patternStringInfo);
}

}  // namespace cyaz
