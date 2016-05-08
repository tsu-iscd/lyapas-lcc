//
// Created by Safonov Vadim on 11/23/15.
//

#ifndef IM1TOIM2_COMPLEXCMD_H
#define IM1TOIM2_COMPLEXCMD_H

#include "../CompositeCmd.h"

class ComplexCmd : public CompositeCmd{
public:
    ComplexCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
    virtual Json::Value toArgumentFormat() override;
};


#endif //IM1TOIM2_COMPLEXCMD_H
