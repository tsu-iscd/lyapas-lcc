#pragma once

#include <stdexcept>
#include <translation_module/aliases.h>
#include <json/json.h>

namespace tests {

Json::Value parse(trm::StringView json)
{
    Json::Value root;
    Json::Reader reader;

    bool ok = reader.parse(json.begin(), json.end(), root);
    if (!ok) {
        throw std::runtime_error{"Ошибка парсинга Json"};
    }

    return root;
}

}
