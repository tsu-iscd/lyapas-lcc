#pragma once

#include <translation_module/translation_module.h>

namespace syaz {
class Steckoyaz : public trm::TranslationModule {
private:
    virtual bool valid(const JSON &cmds, std::string &error) override;
    virtual void preprocess(JSON &cmds) override;
    virtual void postprocess(JSON &cmds) override;

    virtual const trm::Replacers &getReplacers(const JSON &cmds) override;
    virtual std::string getRules() override;
};

}  // namespace syaz
