#include "TreeParser.h"

TreeParser::TreeParser(CmdFactory &cmdFactory)
{
    _cmdFactory = cmdFactory;
}

CompositeCmd::SPtrComposite TreeParser::parseTree(Json::Value &jsonTree)
{
    auto jsonObject = jsonTree[0];
    std::string type = jsonObject["type"].asString();

    CompositeCmd::SPtrComposite rootCompositeCmd = _cmdFactory.createCompositeCmd(type, jsonTree);
    parseTreeAndAddToCmd(jsonTree, rootCompositeCmd);

    return rootCompositeCmd;
}

void TreeParser::parseTreeAndAddToCmd(Json::Value &jsonTree, CompositeCmd::SPtrComposite cmd)
{
    for (int i = 1; i < jsonTree.size(); i++) {
        auto jsonObject = jsonTree[i];
        std::string type = jsonObject[0][fieldName::type].asString();

        CompositeCmd::SPtrComposite childCompositionCmd = _cmdFactory.createCompositeCmd(type, jsonObject);
        if (childCompositionCmd != nullptr) {
            cmd->add(childCompositionCmd);
            parseTreeAndAddToCmd(jsonObject, childCompositionCmd);
            continue;
        }

        ICmd::SPtr childCmd = _cmdFactory.createCmd(type, jsonObject);
        if (childCmd != nullptr) {
            cmd->add(childCmd);
            continue;
        } else {
            throw std::runtime_error("Factory couldn't create Cmd from type: " + type);
        }
    }
}
