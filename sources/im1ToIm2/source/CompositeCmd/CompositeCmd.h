//
// Created by Safonov Vadim on 11/6/15.
//

#ifndef IM1TOIM2_COMPOSITECMD_H
#define IM1TOIM2_COMPOSITECMD_H

#include "ICmd.h"

class CompositeCmd : public ICmd {

public:
    typedef std::shared_ptr<CompositeCmd> SPtrComposite;

    CompositeCmd(Json::Value json);
    virtual void add(const SPtr&);
    virtual void remove(const SPtr&);
    virtual std::string asString();
    virtual Json::Value toJson();

protected:
    std::list<SPtr> _children;
};


#endif //IM1TOIM2_COMPOSITECMD_H
