#pragma once

#include "CompositeCmd.h"

class OperationCmd : public CompositeCmd {
public:
    OperationCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};
