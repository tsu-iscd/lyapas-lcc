#pragma once

#include "../CompositeCmd.h"

class ProgramCmd : public CompositeCmd {
public:
    ProgramCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};
