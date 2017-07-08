#pragma once

#include "LeafCmd.h"

class AsmCmd : public LeafCmd {
public:
    AsmCmd(Json::Value json);

    virtual Json::Value toJson() override;
};
