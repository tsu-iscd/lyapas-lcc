#pragma once

#include <translation_module/translation_module.h>

namespace trm {

class TranslatorFixture : public trm::TranslationModule {
public:
    void setRules(const std::string &newRules);

private:
    virtual bool valid(const JSON &cmds, std::string &error) override;
    virtual void preprocess(JSON &cmds) override;
    virtual void postprocess(JSON &cmds) override;

    virtual const trm::Replacers &getReplacers(const JSON &cmds) override;
    virtual std::string getRules() override;

    std::string rules;
};

}  // namespace trm
