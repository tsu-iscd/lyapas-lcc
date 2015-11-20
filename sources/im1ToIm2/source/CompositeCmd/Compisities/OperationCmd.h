//
// Created by s15-22 on 18.11.15.
//

#ifndef IM1TOIM2_OPERATIONCMD_H
#define IM1TOIM2_OPERATIONCMD_H

#include "../CompositeCmd.h"

class OperationCmd : public CompositeCmd{
public:
    OperationCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};


#endif //IM1TOIM2_OPERATIONCMD_H
