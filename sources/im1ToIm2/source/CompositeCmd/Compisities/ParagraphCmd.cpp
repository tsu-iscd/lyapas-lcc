//
// Created by Safonov Vadim on 11/20/15.
//

#include <iostream>
#include "ParagraphCmd.h"

ParagraphCmd::ParagraphCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value ParagraphCmd::toJson()
{
    Json::Value result;
    result.clear();

    for(SPtr& child : _clidren)
    {
        auto operation = child->toJson();
        result.append(operation);
    }

    return result;
}
