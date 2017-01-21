#pragma once

#include <map>
#include <string>
#include <cstdint>
#include <experimental/optional>

namespace trm {

using JsonMap = std::map<std::string, Json::Value>;
using StringMap = std::map<std::string, std::string>;
using IntMap = std::map<std::string, int64_t>;

template <typename T>
using Optional = std::experimental::optional<T>;

}
