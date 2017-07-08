#pragma once

#include "CompositeCmd.h"

class PrefaceCmd : public CompositeCmd {
public:
    PrefaceCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};
