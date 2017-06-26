#pragma once

#include <CompositeCmd/ICmd.h>

class LeafCmd : public ICmd {
public:
    typedef std::shared_ptr<LeafCmd> SPtrLeaf;

    LeafCmd(Json::Value json);
};
