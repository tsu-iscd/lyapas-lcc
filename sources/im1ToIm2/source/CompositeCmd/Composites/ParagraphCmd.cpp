//
// Created by Safonov Vadim on 11/20/15.
//

#include <iostream>
#include "ParagraphCmd.h"

ParagraphCmd::ParagraphCmd(Json::Value processJson) : CompositeCmd(processJson) {}

Json::Value ParagraphCmd::toJson()
{
    Json::Value result;

    Json::Value paragraphJson;
    paragraphJson[fieldName::type] = "label";
    paragraphJson[fieldName::number] = _cmdJson[fieldName::number];
    result.append(paragraphJson);

    for(SPtr& child : _children)
    {
        result.append(child->toJson());
    }

    return result;
}
