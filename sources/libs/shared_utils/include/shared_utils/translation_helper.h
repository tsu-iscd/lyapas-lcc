#pragma once

#include <json/json.h>
#include "json_helper.h"

namespace sutils {

template <typename T>
void translateSimple(int argc, char *argv[])
{
    Json::Value json;
    if (argc < 2) {
        parseJsonFromStream(std::cin, json);
    } else {
        parseJsonFromFile(argv[1], json);
    }

    T translator;
    translator.translate(json);

    writeJsonToStream(std::cout, json);
}

}  // namespace sutils
