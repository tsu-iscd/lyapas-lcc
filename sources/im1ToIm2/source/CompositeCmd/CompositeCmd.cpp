//
// Created by Safonov Vadim on 11/6/15.
//

#include "CompositeCmd.h"

CompositeCmd::CompositeCmd(Json::Value json) : ICmd(json) { }

void CompositeCmd::add(const SPtr &child)
{
    _clidren.push_back(child);
}

void CompositeCmd::remove(const SPtr &child)
{
    _clidren.remove(child);
}

std::string CompositeCmd::asString()
{
    std::string result;

    result = "Me: " + ICmd::asString() + ";\n";

    int i = 0;
    for(SPtr& child : _clidren)
    {
        result += "Child " + std::to_string(i) + ": " + child->asString() + ";\n";
        i++;
    }

    return result;
}
