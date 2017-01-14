#include <iostream>
#include <fstream>
#include "json/json.h"
#include "TreeParser/IParser.h"
#include "TreeParser/TreeParser.h"

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

int main(int argc, char* argv[])
{
    Json::Value jsonTree;
    if(argc < 2) {
        parseJsonFromStream(std::cin, jsonTree);
    } else {
        parseJsonFromFile(argv[1], jsonTree);
    }

    CmdFactory factory;
    TreeParser treeParser(factory);

    Json::Value jsonResult = treeParser.parseTree(jsonTree)->toJson();
    writeJsonToStream(std::cout, jsonResult);

    return 0;
}