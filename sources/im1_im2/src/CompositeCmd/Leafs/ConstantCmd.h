#pragma once

#include "../LeafCmd.h"

class ConstantCmd : public LeafCmd {
public:
    ConstantCmd(Json::Value json);

    virtual Json::Value toJson() override;
    virtual Json::Value toArgumentFormat() override;
};
