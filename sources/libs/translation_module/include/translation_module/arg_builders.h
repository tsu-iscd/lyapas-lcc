#pragma once

namespace trm {

class ArgBuilder {
public:
    virtual ~ArgBuilder();
    virtual Json::Value createArg() = 0;
};

std::shared_ptr<ArgBuilder> createArgBuilder(const std::string& value) {
    // arg:
    // int, "string" => ConstArgBuilder
    // {} => JsonArgBuilder
    // "<> <>" => PatternArgBuilder

}

}
