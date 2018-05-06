#include "make_cmd.h"

namespace ayaz {

JSON makeCmd(const std::string &name, const std::initializer_list<JSON> &args)
{
    JSON res;
    res["type"] = "cmd";
    res["cmd"] = name;
    res["args"] = JSON{Json::arrayValue};
    for (const JSON &arg : args) {
        res["args"].append(arg);
    }
    return res;
}

JSON makeLabel(const std::string &name)
{
    JSON res;
    res["type"] = "label";
    res["name"] = name;
    return res;
}

}

