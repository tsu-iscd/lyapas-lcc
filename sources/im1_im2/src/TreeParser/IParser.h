#pragma once

#include "CmdFactory.h"
#include <CompositeCmd/Composites/CompositeCmd.h>

class IParser {
public:
    virtual CompositeCmd::SPtrComposite parseTree(Json::Value& jsonTree) = 0;
};
