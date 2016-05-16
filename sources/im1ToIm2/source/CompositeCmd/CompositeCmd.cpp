//
// Created by Safonov Vadim on 11/6/15.
//

#include "CompositeCmd.h"

CompositeCmd::CompositeCmd(Json::Value json) : ICmd(json) { }

void CompositeCmd::add(const SPtr &child)
{
    _children.push_back(child);
}

void CompositeCmd::remove(const SPtr &child)
{
    _children.remove(child);
}

std::string CompositeCmd::asString()
{
    std::string result;

    result = "Me: " + ICmd::asString() + ";\n";

    int i = 0;
    for(SPtr& child : _children)
    {
        result += "Child " + std::to_string(i) + ": " + child->asString() + ";\n";
        i++;
    }

    return result;
}

Json::Value CompositeCmd::toJson()
{
    Json::Value result;
    result.clear();

    result.append(ICmd::toJson());

    for(SPtr& child : _children)
    {
        result.append(child->toJson());
    }

    return result;
}
