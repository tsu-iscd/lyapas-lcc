//
// Created by Safonov Vadim on 11/4/15.
//

#ifndef IM1TOIM2_IPARSER_H
#define IM1TOIM2_IPARSER_H


#include "CmdFactory.h"
#include "../CompositeCmd/CompositeCmd.h"

class IParser {
public:
    virtual CompositeCmd::SPtrComposite parseTree(Json::Value& jsonTree) = 0;
};


#endif //IM1TOIM2_PARSERTREE_H
