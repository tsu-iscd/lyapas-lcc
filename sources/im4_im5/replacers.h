#pragma once

#include <functional>
#include <translation_module/replacers.h>
#include "counters.h"

namespace cyaz {

class FreeVarReplacer : public trm::Replacer {
public:
    FreeVarReplacer(const Json::Value &cmds)
    {
        for (const auto &cmd : cmds) {
            counter.process(cmd);
        }
    }

    void updateState(const Json::Value &cmd) override
    {
        counter.updateState(cmd);
    }

    std::string resolve(const trm::PatternStringInfo &patternStringInfo) override
    {
        trm::Optional<std::string> param = patternStringInfo.getParam();
        if (!param) {
            throw std::runtime_error("Паттерн " + patternStringInfo.getName() + " указан без параметра");
        }

        return 't' + std::to_string(counter.getFreeVariable(std::stoul(*param)));
    }

private:
    VariableCounter counter;
};

class FreeLabelReplacer : public trm::Replacer {
public:
    FreeLabelReplacer(const Json::Value &cmds)
    {
        for (const auto &cmd : cmds) {
            counter.process(cmd);
        }
    }

    void updateState(const Json::Value &cmd) override
    {
        counter.updateState(cmd);
    }

    std::string resolve(const trm::PatternStringInfo &patternStringInfo) override
    {
        trm::Optional<std::string> param = patternStringInfo.getParam();
        if (!param) {
            throw std::runtime_error("Паттерн " + patternStringInfo.getName() + " указан без параметра");
        }

        return std::to_string(counter.getFreeLabel(std::stoul(*param)));
    }

private:
    LabelCounter counter;
};

using ReplaceFunction = std::function<std::string(const trm::PatternStringInfo &patternStringInfo)>;
class FunctionalReplacer : public trm::Replacer {
public:
    FunctionalReplacer(ReplaceFunction function)
        : function(function)
    {
    }

    std::string resolve(const trm::PatternStringInfo &patternStringInfo) override
    {
        return function(patternStringInfo);
    }

private:
    ReplaceFunction function;
};

}  // namespace cyaz
