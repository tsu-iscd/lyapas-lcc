#include <iostream>
#include <fstream>
#include "json/json.h"
#include "TreeParser/IParser.h"
#include "TreeParser/TreeParser.h"

using namespace std;

Json::Value parseJsonFromStream(istream &stream)
{
    Json::Value json;
    Json::Reader reader;

    if(reader.parse(stream, json))
    {
        return json;
    }

    throw std::runtime_error("Json Parse Error: " + reader.getFormattedErrorMessages());
}

Json::Value parseJsonFromFile(const char *name)
{
    ifstream jsonFile;
    jsonFile.exceptions(ifstream::failbit | ifstream::badbit);
    jsonFile.open(name, std::ifstream::binary);

    return parseJsonFromStream(jsonFile);
}

int main(int argc, char* argv[])
{
    Json::Value jsonTree;
    if(argc < 2) {
        jsonTree = parseJsonFromStream(std::cin);
    } else {
        jsonTree = parseJsonFromFile(argv[1]);
    }

    CmdFactory factory;
    TreeParser treeParser(factory);

    auto result = treeParser.parseTree(jsonTree);
    cout << result->toJson().toStyledString() << endl;

    return 0;
}