#pragma once

#include "aliases.h"
#include "replacers.h"

namespace trm {

struct CmdTranslatorStorage {
    const Replacers &replacers;

    JsonMap srcArgJson;
    StringMap srcArgString;
    IntMap srcArgInt;
};

}  // namespace trm
