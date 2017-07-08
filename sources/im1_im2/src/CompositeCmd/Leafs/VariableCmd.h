#pragma once

#include "LeafCmd.h"

class VariableCmd : public LeafCmd {
public:
    VariableCmd(Json::Value json);

    virtual Json::Value toJson() override;
    virtual Json::Value toArgumentFormat() override;
};
