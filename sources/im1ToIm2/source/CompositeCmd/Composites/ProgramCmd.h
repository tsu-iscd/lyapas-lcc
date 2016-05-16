//
// Created by vosafonov on 27.03.16.
//

#ifndef IM1TOIM2_PROGRAMCMD_H
#define IM1TOIM2_PROGRAMCMD_H

#include "../CompositeCmd.h"

class ProgramCmd: public CompositeCmd{
public:
    ProgramCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};


#endif //IM1TOIM2_PROGRAMCMD_H
