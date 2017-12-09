#include <json/json.h>
#include <shared_utils/translation_helper.h>
#include "TreeParser/IParser.h"
#include "TreeParser/TreeParser.h"

class Odnoyaz {
public:
    Odnoyaz() {}

    void translate(Json::Value &json)
    {
        CmdFactory factory;
        TreeParser treeParser(factory);
        json = treeParser.parseTree(json)->toJson();
    }
};

int main(int argc, char *argv[])
{
    sutils::translateSimple<Odnoyaz>(argc, argv);
    return 0;
}
