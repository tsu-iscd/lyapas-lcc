//
// Created by Safonov Vadim on 11/6/15.
//

#ifndef IM1TOIM2_COMPOSITECMD_H
#define IM1TOIM2_COMPOSITECMD_H

#include "ICmd.h"

class CompositeCmd : public ICmd {

public:
    CompositeCmd(Json::Value json);
    virtual void add(const SPtr&);
    virtual void remove(const SPtr&);
    std::string asString();

protected:
    std::list<SPtr> _clidren;
};


#endif //IM1TOIM2_COMPOSITECMD_H
