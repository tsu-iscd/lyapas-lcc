//
// Created by s15-22 on 21.11.15.
//

#ifndef IM1TOIM2_VARIABLECMD_H
#define IM1TOIM2_VARIABLECMD_H

#include "../LeafCmd.h"

class VariableCmd : public LeafCmd {
public:
    VariableCmd(Json::Value json);

    virtual Json::Value toJson();
};


#endif //IM1TOIM2_VARIABLECMD_H
