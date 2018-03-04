#pragma once

#include <functional>
#include <string>
#include <translation_module/replacers.h>

namespace cyaz {

using ReplaceFunction = std::function<std::string(const trm::PatternStringInfo &patternStringInfo)>;

class FunctionalReplacer : public trm::Replacer {
public:
    explicit FunctionalReplacer(ReplaceFunction function);

    std::string resolve(const trm::PatternStringInfo &patternStringInfo) override;

private:
    ReplaceFunction function;
};

}  // namespace cyaz
