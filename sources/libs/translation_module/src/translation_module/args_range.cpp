#include "args_range.h"
#include <algorithm>
#include <shared_utils/assertion.h>

namespace trm {

ArgsRange::ArgsRange(const Filters &filters, JSON &cmd)
{
    LCC_ASSERT(cmd.isObject());

    //
    // проверяем содержимое команды
    //
    bool isCmd = cmd.isMember("type") && cmd["type"] == "cmd" && cmd.isMember("cmd") && cmd["cmd"].isString();
    bool hasArgs = cmd.isMember("args");
    if (!isCmd || !hasArgs) {
        return;
    }

    //
    // ищем фильтр
    //
    JSON &args = cmd["args"];
    const std::string cmdName = cmd["cmd"].asString();
    auto found = filters.find(cmdName);
    if (found == filters.end()) {
        //
        // пропускаем фильтрацию
        //
        std::transform(std::begin(args), std::end(args), std::back_inserter(filteredArgs), [](JSON &json) { return &json; });
        return;
    }
    const ArgsFilter &filter = found->second;

    //
    // фильтруем аргументы
    //

    switch (filter.ignore) {
    case ArgsFilter::Ignore::ALL:
        break;
    case ArgsFilter::Ignore::SOME:
        for (int i = 0; i < args.size(); ++i) {
            auto found = filter.ignoreArgs.find(i);
            if (found == filter.ignoreArgs.end()) {
                filteredArgs.push_back(&args[i]);
            }
        }
        break;
    case ArgsFilter::Ignore::NAME_FUNCTION_AND_SLASH:
        for (int i = 1; i < args.size(); ++i) {
            if (args[i].asString() != "/") {
                filteredArgs.push_back(&args[i]);
            }
        }
        break;
    default:
        throw std::logic_error{"Unexpected case"};
    }
}

}  // namespace trm
