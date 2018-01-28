#pragma once

#include <algorithm>
#include <translation_module/translation_module.h>
#include "FunctionInfo.h"

namespace syaz {
class Steckoyaz : public trm::TranslationModule {
private:
    virtual bool valid(const JSON &cmds, std::string &error) override;
    virtual void preprocess(JSON &cmds) override;
    virtual void postprocess(JSON &cmds) override;

    virtual const trm::Replacers &getReplacers(const JSON &cmds) override;
    virtual std::string getRules() override;

    void translateCall(const JSON &cmd, JSON &resultCmds);
    void translateDefinition(JSON &function, JSON &resultCmds);
    int countLocalVariables(JSON &function, std::map<std::string, int> &variables);
    Json::Value stackAlloc(int shift);
    Json::Value stackFree(int shift);
};

}  // namespace syaz
