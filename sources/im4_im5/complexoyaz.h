#pragma once

#include <translation_module/translation_module.h>

namespace cyaz {

class Complexoyaz : public trm::TranslationModule {
private:
    virtual bool valid(const JSON &cmds, std::string &error) override;
    virtual void preprocess(JSON &cmds) override;
    virtual void postprocess(JSON &cmds) override;

    virtual trm::Replacers makeReplacers() override;
    virtual std::string getRules() override;

    JSON processFunctions(JSON &cmds);

    friend class ComplexoyazFixture;
};

}  // namespace cyaz
