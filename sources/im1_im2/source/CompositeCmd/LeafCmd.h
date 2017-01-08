//
// Created by Safonov Vadim on 11/6/15.
//

#ifndef IM1TOIM2_LEAFCMD_H
#define IM1TOIM2_LEAFCMD_H

#include "ICmd.h"

class LeafCmd : public ICmd {
public:
    typedef std::shared_ptr<LeafCmd> SPtrLeaf;

    LeafCmd(Json::Value json);
};


#endif //IM1TOIM2_LEAFCMD_H
