#pragma once

#include "CompositeCmd.h"

class ComplexCmd : public CompositeCmd {
public:
    ComplexCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
    virtual Json::Value toArgumentFormat() override;
};
