#pragma once

#include <translation_module/translation_module.h>
#include "function.h"

namespace syaz {
class Stackoyaz : public trm::TranslationModule {
private:
    virtual bool valid(const JSON &cmds, std::string &error) override;
    virtual void preprocess(JSON &cmds) override;
    virtual void postprocess(JSON &cmds) override;

    virtual trm::Replacers makeReplacers() override;
    virtual std::string getRules() override;

    void translateCall(Function &func);
    void translateDefinition(Function &func);
    void translateLabels(Function &func);
};

}  // namespace syaz
