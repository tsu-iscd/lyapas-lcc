//
// Created by vosafonov on 15.05.16.
//

#ifndef IM1TOIM2_STRINGCMD_H
#define IM1TOIM2_STRINGCMD_H

#include "../LeafCmd.h"

class StringCmd : public LeafCmd {
public:
    StringCmd(Json::Value json);

    virtual Json::Value toJson() override;
    virtual Json::Value toArgumentFormat() override;
};


#endif //IM1TOIM2_STRINGCMD_H
