#include <iostream>
#include <fstream>
#include "json/json.h"
#include "source/CompositeCmd/Compisities/ProcessCmd.h"

using namespace std;

int readAndBuildTest()
{
    Json::Value root;
    Json::Reader reader;

    std::ifstream file("/Users/karmiclabs/work/gits/lcc/sources/im1ToIm2/ex3.im1o", std::ifstream::binary);
    bool parsingSuccessful = reader.parse(file, root);

    if(!parsingSuccessful)
    {
        cout << reader.getFormattedErrorMessages();
        return 1;
    }

    ProcessCmd processCmd(root);
    cout << "Result: " << processCmd.asString();

    return 0;
}

int main() {
    readAndBuildTest();
    return 0;
}