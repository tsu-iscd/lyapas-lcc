#pragma once

#include "CompositeCmd.h"

class ArgsCmd : public CompositeCmd {
public:
    ArgsCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};
