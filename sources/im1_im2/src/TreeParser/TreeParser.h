#pragma once

#include "IParser.h"

class TreeParser : public IParser {
public:
    TreeParser(CmdFactory &cmdFactory);
    CompositeCmd::SPtrComposite parseTree(Json::Value& jsonTree) override;

private:
    void parseTreeAndAddToCmd(Json::Value &jsonTree, CompositeCmd::SPtrComposite cmd);

    CmdFactory _cmdFactory;
};
