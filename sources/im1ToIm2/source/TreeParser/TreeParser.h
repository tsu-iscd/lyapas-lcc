//
// Created by s15-22 on 07.11.15.
//

#ifndef IM1TOIM2_TREEPARSER_H
#define IM1TOIM2_TREEPARSER_H


#include "IParser.h"

class TreeParser : public IParser {
public:
    TreeParser(CmdFactory &cmdFactory);
    CompositeCmd::SPtrComposite parseTree(Json::Value& jsonTree) override;

private:
    void parseTreeAndAddToCmd(Json::Value &jsonTree, CompositeCmd::SPtrComposite cmd);

    CmdFactory _cmdFactory;
};


#endif //IM1TOIM2_TREEPARSER_H
