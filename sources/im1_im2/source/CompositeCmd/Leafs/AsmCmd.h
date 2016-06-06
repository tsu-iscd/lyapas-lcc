//
// Created by vosafonov on 16.05.16.
//

#ifndef IM1TOIM2_ASMCMD_H
#define IM1TOIM2_ASMCMD_H

#include "../LeafCmd.h"

class AsmCmd : public LeafCmd {
public:
    AsmCmd(Json::Value json);

    virtual Json::Value toJson() override;
};


#endif //IM1TOIM2_ASMCMD_H
