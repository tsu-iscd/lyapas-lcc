#include "function_signature.h"
#include <algorithm>
#include <shared_utils/assertion.h>

FunctionSignature::FunctionSignature(const JSON &cmd)
{
    const JSON &args = cmd["args"];
    LCC_ASSERT(args.size() != 0);

    name = args[0].asString();

    auto slash = std::find(args.begin(), args.end(), "/");
    LCC_ASSERT(slash != args.end());

    input = std::vector<JSON>{++args.begin(), slash};
    output = std::vector<JSON>{++slash, args.end()};
}