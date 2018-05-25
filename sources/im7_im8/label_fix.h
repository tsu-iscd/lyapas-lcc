#pragma once

//
// временные функции, они будут удалены после того,
// как метки будут иметь префикс с именем функции
//

#include <algorithm>
#include <cctype>
#include <set>
#include <string>
#include <shared_utils/assertion.h>
#include <translation_module/aliases.h>

namespace ayaz {
namespace tofix {

const std::string labelPrefix = "_label_";

inline bool isNumber(const std::string &s)
{
    return !s.empty() && std::find_if(std::begin(s),
                                      std::end(s),
                                      [](char c) { return !std::isdigit(c); }) == s.end();
}

inline void fixLabel(JSON &cmd)
{
    JSON *labelName = nullptr;

    if (cmd.isMember("number")) {
        labelName = &cmd["number"];
    } else if (cmd.isMember("name")) {
        labelName = &cmd["name"];
    } else if (cmd.isMember("args")) {
        labelName = &cmd["args"][0];
    } else {
        LCC_FAIL("Unexpected case");
    }

    if (isNumber(labelName->asString())) {
        *labelName = labelPrefix + labelName->asString();
    }
}

inline void fixJump(JSON &cmd)
{
    std::set<std::string> jumps{
        "jmp",
        "jb",
        "jbe",
        "ja",
        "jae",
        "je",
        "jne",
    };

    LCC_ASSERT(cmd.isMember("cmd"));
    std::string jumpName = cmd["cmd"].asString();
    auto found = jumps.find(jumpName);
    LCC_ASSERT(found != std::end(jumps));

    LCC_ASSERT(cmd.isMember("args"));
    LCC_ASSERT(cmd["args"].size() == 1);
    cmd["args"][0] = labelPrefix + cmd["args"][0].asString();
}

}
}
