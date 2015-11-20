//
// Created by Safonov Vadim on 11/20/15.
//

#include "ParagraphCmd.h"

ParagraphCmd::ParagraphCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value ParagraphCmd::toJson()
{
    Json::Value result;
    result.clear();

    for(SPtr& child : _clidren)
    {
        auto paragraph = child->toJson();
        for(auto cmd : paragraph)
        {
            result.append(cmd);
        }
    }

    return result;
}
