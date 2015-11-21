//
// Created by Safonov Vadim on 11/6/15.
//

#ifndef IM1TOIM2_STABLEAFCMD_H
#define IM1TOIM2_STABLEAFCMD_H


#include "../LeafCmd.h"

class StabLeafCmd : public LeafCmd {
public:
    StabLeafCmd(Json::Value json);

    virtual Json::Value toJson();
};


#endif //IM1TOIM2_STABLEAFCMD_H
