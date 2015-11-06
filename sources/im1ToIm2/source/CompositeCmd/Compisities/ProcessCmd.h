//
// Created by Safonov Vadim on 11/6/15.
//

#ifndef IM1TOIM2_PROCCMD_H
#define IM1TOIM2_PROCCMD_H


#include "../CompositeCmd.h"

class ProcessCmd : public CompositeCmd{
public:
    ProcessCmd(Json::Value processJson);
};


#endif //IM1TOIM2_PROCCMD_H
