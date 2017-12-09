#pragma once

#include <CompositeCmd/Composites/CompositeCmd.h>
#include "CmdFactory.h"

class IParser {
public:
    virtual CompositeCmd::SPtrComposite parseTree(Json::Value &jsonTree) = 0;
};
