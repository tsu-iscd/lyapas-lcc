#include "program.h"

namespace ayaz {

Program transform(const JSON &cmds)
{
    return {cmds.begin(), cmds.end()};
}

JSON transform(const Program &program)
{
    JSON cmds{Json::arrayValue};

    for (auto &cmd : program) {
        cmds.append(cmd);
    }

    return cmds;
}

}
