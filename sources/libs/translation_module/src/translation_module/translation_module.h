#pragma once

#include <stdexcept>
#include <string>
#include <json/json.h>
#include "aliases.h"
#include "replacers.h"

namespace trm {

class TranslationModule {
public:
    ~TranslationModule();

    void translate(JSON &cmds);

private:
    virtual bool valid(const JSON &cmds, std::string &error);
    virtual void preprocess(JSON &cmds) = 0;
    // поведение этой функции опеределяется конфиг файлами трансляции
    virtual void process(JSON &cmds) final;
    virtual void postprocess(JSON &cmds) = 0;

    Replacers &getReplacers(const JSON &cmds);
    virtual Replacers makeReplacers() = 0;

    virtual std::string getRules() = 0;

    Optional<Replacers> replacers;
};

}  // namespace trm
