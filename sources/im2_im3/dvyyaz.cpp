#include "dvyyaz.h"

bool dyaz::Dvyyaz::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void dyaz::Dvyyaz::preprocess(JSON &cmds)
{
}

void dyaz::Dvyyaz::postprocess(JSON &cmds)
{
}

const trm::Replacers &dyaz::Dvyyaz::getReplacers()
{
    static trm::Replacers replacers;
    return replacers;
}

std::string dyaz::Dvyyaz::getRules()
{
    return std::string(
#include "rules.txt"
    );
}
