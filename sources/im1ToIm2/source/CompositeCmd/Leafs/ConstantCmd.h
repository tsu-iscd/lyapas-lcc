//
// Created by s15-22 on 21.11.15.
//

#ifndef IM1TOIM2_CONSTANTCMD_H
#define IM1TOIM2_CONSTANTCMD_H


#include "../LeafCmd.h"

class ConstantCmd : public LeafCmd {
public:
    ConstantCmd(Json::Value json);

    virtual Json::Value toJson() override;
    virtual Json::Value toArgumentFormat() override;
};


#endif //IM1TOIM2_CONSTANTCMD_H
