#include "function_info.h"

FunctionInfo::FunctionInfo(const JSON &cmd)
{
    const JSON &args = cmd["args"];
    LCC_ASSERT(args.size() != 0);

    name = args.operator[](0).asString();

    auto slash = std::find(args.begin(), args.end(), "/");
    LCC_ASSERT(slash != args.end());

    // FIXME как от этого уйти?
    auto s = args.begin();
    s++;
    input = std::vector<JSON>(s, slash);
    slash++;
    output = std::vector<JSON>(slash, args.end());
}