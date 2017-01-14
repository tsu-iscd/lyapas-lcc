#include <iostream>
#include <json/json.h>
#include <shared_utils/json_helper.h>
#include "TreeParser/IParser.h"
#include "TreeParser/TreeParser.h"

int main(int argc, char* argv[])
{
    Json::Value jsonTree;
    if(argc < 2) {
        sutils::parseJsonFromStream(std::cin, jsonTree);
    } else {
        sutils::parseJsonFromFile(argv[1], jsonTree);
    }

    CmdFactory factory;
    TreeParser treeParser(factory);

    Json::Value jsonResult = treeParser.parseTree(jsonTree)->toJson();
    sutils::writeJsonToStream(std::cout, jsonResult);

    return 0;
}
