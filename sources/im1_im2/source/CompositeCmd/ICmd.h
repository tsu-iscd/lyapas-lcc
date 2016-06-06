//
// Created by Safonov Vadim on 11/4/15.
//

#ifndef IM1TOIM2_ICMD_H
#define IM1TOIM2_ICMD_H

#include <memory>
#include <list>
#include <stdexcept>
#include <string>
#include <json/json.h>

class ICmd {

protected:
    Json::Value _cmdJson;

public:
    typedef std::shared_ptr<ICmd> SPtr;

    ICmd(Json::Value json);

    virtual std::string asString();
    virtual Json::Value toJson();
    virtual Json::Value toArgumentFormat();
};

namespace fieldName {
const std::string cmd = "cmd";
const std::string cmd_postfix = "cmd_postfix";
const std::string args = "args";
const std::string value = "value";
const std::string name = "name";
const std::string type = "type";
const std::string number = "number";
const std::string command = "command";
}

#endif //IM1TOIM2_COMPOSITECMD_H