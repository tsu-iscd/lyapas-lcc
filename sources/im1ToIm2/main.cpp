#include <iostream>
#include <fstream>
#include "json/json.h"
#include "source/CompositeCmd/Compisities/ProcedureCmd.h"
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

void parseTree(CompositeCmd::SPtrComposite cmd, Json::Value root)
{
    for(int i = 1; i < root.size(); i++)
    {
        CompositeCmd::SPtrComposite newCmd(new CompositeCmd(root[i]));
        cmd->add(newCmd);
        parseTree(newCmd, root[i]);
    }
}
//*** END TEST PARSE ***/

int main(int argc, char* argv[])
{
    Json::Value json;
    if(argc < 2)
    {
        std::cin >> json;
    }
    else
    {
        string pathToFile(argv[1]);

        ifstream jsonFile;
        jsonFile.open(pathToFile, std::ifstream::binary);
        json = tryParseJson(jsonFile); //TODO: exit if parse error
    }

    CmdFactory factory;
    TreeParser treeParser(factory);

    auto res = treeParser.parseTree(json);

    bool printAsString = false;
    if(printAsString)
    {
        cout << res->asString() << endl;
    }
    else
    {
        cout << res->toJson().toStyledString() << endl;
    }

    return 0;
}