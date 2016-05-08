//
// Created by vosafonov on 27.03.16.
//

#ifndef IM1TOIM2_ARGSCMD_H
#define IM1TOIM2_ARGSCMD_H

#include "../CompositeCmd.h"

class ArgsCmd: public CompositeCmd{
public:
    ArgsCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};


#endif //IM1TOIM2_ARGSCMD_H
