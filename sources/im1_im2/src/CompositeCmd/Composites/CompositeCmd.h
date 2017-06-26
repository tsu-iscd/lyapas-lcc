#pragma once

#include <CompositeCmd/ICmd.h>

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
