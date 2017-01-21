#pragma once

#include <translation_module/translation_module.h>

namespace dyaz {

class Dvyyaz : public trm::TranslationModule {
private:
    virtual bool valid(const JSON &cmds, std::string &error) override;
    virtual void preprocess(JSON &cmds) override;
    virtual void postprocess(JSON &cmds) override;

    virtual const trm::Replacers &getReplacers() override;
    virtual std::string getRules() override;
};

}
