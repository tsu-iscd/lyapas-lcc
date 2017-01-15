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

std::string dyaz::Dvyyaz::getDesignations()
{
    return std::string(
#include "designations.txt"
    );
}

std::string dyaz::Dvyyaz::getRules()
{
    return std::string(
#include "rules.txt"
    );
}
