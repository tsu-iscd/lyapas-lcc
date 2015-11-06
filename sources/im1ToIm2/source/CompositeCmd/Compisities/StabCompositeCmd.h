//
// Created by Safonov Vadim on 11/6/15.
//

#ifndef IM1TOIM2_STABCOMPOSITECMD_H
#define IM1TOIM2_STABCOMPOSITECMD_H


#include "../CompositeCmd.h"

class StabCompositeCmd : public CompositeCmd {
public:
    StabCompositeCmd(Json::Value json);
};


#endif //IM1TOIM2_STABCOMPOSITECMD_H
