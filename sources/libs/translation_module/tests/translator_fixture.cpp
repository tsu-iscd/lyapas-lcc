#include "translator_fixture.h"

namespace trm {

void TranslatorFixture::setRules(const std::string &newRules)
{
    rules = newRules;
}

bool TranslatorFixture::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void TranslatorFixture::preprocess(JSON &cmds) {}

void TranslatorFixture::postprocess(JSON &cmds) {}

const trm::Replacers &TranslatorFixture::getReplacers(const JSON &cmds)
{
    static trm::Replacers replacers;
    return replacers;
}

std::string TranslatorFixture::getRules()
{
    return rules;
}

}  // namespace trm
