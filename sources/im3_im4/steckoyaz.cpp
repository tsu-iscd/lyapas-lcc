#include "steckoyaz.h"

namespace syaz {
bool Steckoyaz::valid(const JSON &cmds, std::string &errror)
{
    return true;
}

void Steckoyaz::preprocess(JSON &cmds) {}

void Steckoyaz::postprocess(JSON &cmds) {}

trm::Replacers &Steckoyaz::getReplacers(const JSON &cmds) {
    static trm::Replacers replacers;
    return replacers;
}

std::string Steckoyaz::getRules()
{
    return std::string("");
}
}  // namespace syaz
