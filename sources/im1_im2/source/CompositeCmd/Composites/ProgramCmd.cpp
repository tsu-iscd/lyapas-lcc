//
// Created by vosafonov on 27.03.16.
//

#include "ProgramCmd.h"

ProgramCmd::ProgramCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value ProgramCmd::toJson()
{
    Json::Value result;

    for(SPtr& child : _children) {
        auto paragraph = child->toJson();
        for(auto cmd : paragraph) {
            result.append(cmd);
        }
    }

    return result;
}
