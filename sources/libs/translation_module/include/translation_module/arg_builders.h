#pragma once

namespace trm {

class ArgBuilder {
public:
    virtual ~ArgBuilder();
    virtual Json::Value createArg() = 0;
};

}
