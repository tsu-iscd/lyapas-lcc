#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include "aliases.h"

namespace trm {

struct ArgsFilter {
    enum class Ignore {
        ALL,
        SOME,
        NAME_FUNCTION,
        SLASH,
        NAME_FUNCTION_SLASH,
    };

    Ignore ignore;
    std::set<size_t> ignoreArgs;
};

using Filters = std::unordered_map<std::string, ArgsFilter>;

class ArgsRange {
public:
    ArgsRange(const Filters &filters, JSON &cmd);

    auto begin()
    {
        return filteredArgs.begin();
    }

    auto end()
    {
        return filteredArgs.end();
    }

    std::vector<JSON *> filteredArgs;
};

}  // namespace trm
