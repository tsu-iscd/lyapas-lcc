//
// Created by s15-22 on 25.11.15.
//

#ifndef IM1TOIM2_PREFACECMD_H
#define IM1TOIM2_PREFACECMD_H

#include "../CompositeCmd.h"

class PrefaceCmd : public CompositeCmd {
public:
    PrefaceCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
};


#endif //IM1TOIM2_PREFACECMD_H
