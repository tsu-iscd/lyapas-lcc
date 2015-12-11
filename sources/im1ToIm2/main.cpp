#include <iostream>
#include <fstream>
#include "json/json.h"
#include "source/CompositeCmd/Compisities/ProcessCmd.h"
#include "source/TreeParser/IParser.h"
#include "source/TreeParser/TreeParser.h"

using namespace std;

//***TEST PARSE: Рекрсивный парсер дерева. которое затем выводится в консоль
// В филнальной версии парсер будет реализован в отдельном классе и для каждого узла дерева
// будет создавать нужную реализацию ICmd.
// ***/
void parseTree(CompositeCmd::SPtrComposite cmd, Json::Value root)
{
    for(int i = 1; i < root.size(); i++)
    {
        CompositeCmd::SPtrComposite newCmd(new CompositeCmd(root[i]));
        cmd->add(newCmd);
        parseTree(newCmd, root[i]);
    }
}

void readFile(string pathToFile, ifstream& file)
{
    file.open(pathToFile, std::ifstream::binary);
}

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

void printCmds(Json::Value root)
{
    CompositeCmd::SPtrComposite rootCmd(new ProcessCmd(root));
    parseTree(rootCmd, root);
    cout << "Result: " << rootCmd->asString();
}

void tryPrintCmds(Json::Value json)
{
    if(!json.isNull())
    {
        printCmds(json);
    }
}
//*** END TEST PARSE ***/

int main(int argc, char* argv[])
{
    Json::Value json;
    if(argc < 2)
    {
        cin >> json;
    }
    else
    {
        string pathToFile(argv[1]);

        ifstream jsonFile;
        readFile(pathToFile, jsonFile);
        json = tryParseJson(jsonFile);
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

    //Старый способ вывода
    //tryPrintCmds(json);

    return 0;
}