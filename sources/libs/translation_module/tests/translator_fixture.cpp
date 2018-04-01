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

Replacers TranslatorFixture::makeReplacers()
{
    return {};
}

std::string TranslatorFixture::getRules()
{
    return rules;
}

}  // namespace trm
