#include <iostream>
#include <fstream>
#include "json/json.h"
#include "source/TreeParser/IParser.h"
#include "source/TreeParser/TreeParser.h"

using namespace std;

//***TEST PARSE: Рекрсивный парсер дерева. которое затем выводится в консоль
// В филнальной версии парсер будет реализован в отдельном классе и для каждого узла дерева
// будет создавать нужную реализацию ICmd.
// ***/
Json::Value tryParseJson(istream &stream)
{
    Json::Value json;
    Json::Reader reader;

    bool parsingSuccessful = reader.parse(stream, json);
    if(!parsingSuccessful)
    {
        cout << "Json Parse Error: " << endl;
        cout << reader.getFormattedErrorMessages() << endl;
    }

    return json;
}
//*** END TEST PARSE ***/

int main(int argc, char* argv[])
{
    Json::Value jsonTree;
    if(argc < 2)
    {
        std::cin >> jsonTree;
    }
    else
    {
        string pathToFile(argv[1]);

        ifstream jsonFile;
        jsonFile.open(pathToFile, std::ifstream::binary);
        //TODO:
        //This functions doesn't look realy good
        //it tries to parse json, but if it can't parse json
        //it will print message and nothing more
        jsonTree = tryParseJson(jsonFile);
        //Maybe there should be a reaction(for failed parsing)
    }

    CmdFactory factory;
    TreeParser treeParser(factory);

    auto result = treeParser.parseTree(jsonTree);
    cout << result->toJson().toStyledString() << endl;

    return 0;
}