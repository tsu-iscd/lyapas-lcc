#include "dvyyaz.h"

namespace dyaz {

bool Dvyyaz::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Dvyyaz::preprocess(JSON &cmds) {}

void Dvyyaz::postprocess(JSON &cmds) {}

trm::Replacers Dvyyaz::makeReplacers()
{
    return {};
}

std::string Dvyyaz::getRules()
{
    return std::string(
#include "rules.txt"
    );
}

}  // namespace dyaz
