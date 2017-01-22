#include "dvyyaz.h"

namespace dyaz {

bool Dvyyaz::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Dvyyaz::preprocess(JSON &cmds)
{
}

void Dvyyaz::postprocess(JSON &cmds)
{
}

const trm::Replacers &Dvyyaz::getReplacers(const JSON &cmds)
{
    static trm::Replacers replacers;
    return replacers;
}

std::string Dvyyaz::getRules()
{
    return std::string(
#include "rules.txt"
    );
}

}
