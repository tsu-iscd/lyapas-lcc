#pragma once

#include <stdexcept>
#include <json/json.h>
#include <translation_module/aliases.h>

namespace tests {

inline Json::Value parse(trm::StringView json)
{
    Json::Value root;
    Json::Reader reader;

    bool ok = reader.parse(json.begin(), json.end(), root);
    if (!ok) {
        throw std::runtime_error{"Ошибка парсинга Json"};
    }

    return root;
}

}  // namespace tests
