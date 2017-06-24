#pragma once

#include "CompositeCmd.h"

class ParagraphCmd : public CompositeCmd {
public:
    ParagraphCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};
