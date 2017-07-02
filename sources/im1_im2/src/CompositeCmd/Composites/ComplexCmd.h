#pragma once

#include "CompositeCmd.h"
#include <unordered_map>

class ComplexCmd : public CompositeCmd {
public:
    ComplexCmd(Json::Value processJson);

    virtual Json::Value toJson() override;
    virtual Json::Value toArgumentFormat() override;

private:
    std::string getComplexName(Json::Value &complexJson) const;

    std::unordered_map<std::string, std::string> nameMap;
};
