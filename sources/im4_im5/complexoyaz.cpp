#include "complexoyaz.h"
#include "replacers.h"

namespace cyaz {

bool Complexoyaz::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Complexoyaz::preprocess(JSON &cmds)
{
}

void Complexoyaz::postprocess(JSON &cmds)
{
}

trm::Replacers &Complexoyaz::getReplacers(const JSON &cmds)
{
    static trm::Replacers replacers;
    return replacers;
}

std::string Complexoyaz::getRules()
{
    return std::string(
#include "rules.txt"
    );
}

}
