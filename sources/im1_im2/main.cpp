#include <iostream>
#include <fstream>
#include "json/json.h"
#include "TreeParser/IParser.h"
#include "TreeParser/TreeParser.h"

using namespace std;

void parseJsonFromStream(istream &stream, Json::Value &json)
{
    Json::Reader reader;
    if(!reader.parse(stream, json))
    {
        throw std::runtime_error("Json Parse Error: " + reader.getFormattedErrorMessages());
    }
}

void parseJsonFromFile(const char *name, Json::Value &json)
{
    ifstream jsonFile;
    jsonFile.exceptions(ifstream::failbit | ifstream::badbit);
    jsonFile.open(name, std::ifstream::binary);

    parseJsonFromStream(jsonFile, json);
}

void writeJsonToStream(ostream &stream, const Json::Value &json)
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
    writeJsonToStream(cout, jsonResult);

    return 0;
}