#include "json_helper.h"

namespace sutils {

void parseJsonFromStream(std::istream &stream, Json::Value &json)
{
    Json::Reader reader;
    if(!reader.parse(stream, json))
    {
        throw std::runtime_error("Json Parse Error: " + reader.getFormattedErrorMessages());
    }
}

void parseJsonFromFile(const char *name, Json::Value &json)
{
    std::ifstream jsonFile;
    jsonFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    jsonFile.open(name, std::ifstream::binary);

    parseJsonFromStream(jsonFile, json);
}

void writeJsonToStream(std::ostream &stream, const Json::Value &json)
{
    stream << json.toStyledString();
}

}
