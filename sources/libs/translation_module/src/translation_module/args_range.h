#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <shared_utils/assertion.h>
#include <translation_module/aliases.h>

namespace trm {

struct ArgsFilter {
    enum class Ignore {
        ALL,
        SOME,
    };

    Ignore ignore;
    std::set<size_t> ignoreArgs;
};

using Filters = std::unordered_map<std::string, ArgsFilter>;

class ArgsRange {
public:
    ArgsRange(const Filters &filters, JSON &cmd)
    {
        if (!cmd.isObject()) {
            return;
        }

        //
        // проверяем содержимое команды
        //
        bool isCmd = cmd.isMember("type") && cmd["type"] == "cmd" && cmd.isMember("cmd") && cmd["cmd"].isString();
        bool hasArgs = cmd.isMember("args");
        if (!isCmd || !hasArgs) {
            return;
        }

        // ищем фильтр
        JSON &args = cmd["args"];
        const std::string cmdName = cmd["cmd"].asString();
        auto found = filters.find(cmdName);
        if (found == filters.end()) {
            std::transform(std::begin(args), std::end(args), std::back_inserter(filteredArgs), std::addressof<JSON>);
            return;
        }
        const ArgsFilter &filter = found->second;

        // фильтруем аргументы
        switch (filter.ignore) {
        case ArgsFilter::Ignore::ALL:
            break;
        case ArgsFilter::Ignore::SOME: {
            for (int i = 0; i < args.size(); ++i) {
                auto found = filter.ignoreArgs.find(i);
                if (found == filter.ignoreArgs.end()) {
                    filteredArgs.push_back(&args[i]);
                }
            }
            break;
        }
        default:
            throw std::logic_error{"Unexpected case"};
        }
    }

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
