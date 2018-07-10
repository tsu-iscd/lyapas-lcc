#pragma once

#include <translation_module/translation_module.h>
#include "program.h"

namespace ayaz {

class Assembler : public trm::TranslationModule {
private:
    virtual bool valid(const JSON &cmds, std::string &error) override;
    virtual void preprocess(JSON &cmds) override;
    virtual void postprocess(JSON &cmds) override;

    virtual trm::Replacers makeReplacers() override;
    virtual std::string getRules() override;

    void processArgs(Program &program);
    void processCmdsConstrains(Program &program);
    void processMovWithDifferentArgsSizes(Program &program);
};

}

