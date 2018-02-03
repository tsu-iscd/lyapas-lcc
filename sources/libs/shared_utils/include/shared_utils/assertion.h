#pragma once

#include <iostream>
#include <stdexcept>

#define LCC_ASSERT(cond)                      \
    if (!(cond)) {                            \
        lcc::fail(__FILE__, __LINE__, #cond); \
    }

#define LCC_FAIL(msg) lcc::fail(__FILE__, __LINE__, msg)

#define LCC_ENSURE(cond, msg)                                                               \
    if (!(cond)) {                                                                          \
        throw std::runtime_error{lcc::makeFailMessage(__FILE__, __LINE__, #cond ", " msg)}; \
    }

namespace lcc {

inline std::string makeFailMessage(const char *file, int line, const char *msg)
{
    // "file(line): invariant violation 'msg'"
    std::string res = file;
    res += '(' + std::to_string(line) + "): ";
    res += "invariant violation '" + std::string{msg} + "'";
    return res;
}

[[noreturn]] inline void fail(const char *file, int line, const char *msg)
{
    std::cout << makeFailMessage(file, line, msg) << std::endl;
    abort();
}

}  // namespace lcc
