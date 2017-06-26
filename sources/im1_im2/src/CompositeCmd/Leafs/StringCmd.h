#pragma once

#include "LeafCmd.h"

class StringCmd : public LeafCmd {
public:
    StringCmd(Json::Value json);

    virtual Json::Value toJson() override;
    virtual Json::Value toArgumentFormat() override;
};
