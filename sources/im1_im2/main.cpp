#include <iostream>
#include <fstream>
#include "json/json.h"
#include "TreeParser/IParser.h"
#include "TreeParser/TreeParser.h"

using namespace std;

Json::Value tryParseJson(istream &stream)
{
    Json::Value json;
    Json::Reader reader;

    if(reader.parse(stream, json))
    {
        return json;
    }

    throw std::runtime_error("Json Parse Error: " + reader.getFormattedErrorMessages());
}

int main(int argc, char* argv[])
{
    Json::Value jsonTree;
    if(argc < 2)
    {
        jsonTree = tryParseJson(std::cin);
    }
    else
    {
        string pathToFile(argv[1]);

        ifstream jsonFile;
        jsonFile.open(pathToFile, std::ifstream::binary);
        jsonTree = tryParseJson(jsonFile);
    }

    CmdFactory factory;
    TreeParser treeParser(factory);

    auto result = treeParser.parseTree(jsonTree);
    cout << result->toJson().toStyledString() << endl;

    return 0;
}