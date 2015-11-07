//
// Created by Safonov Vadim on 11/4/15.
//

#include "ICmd.h"

ICmd::ICmd(Json::Value json) : _cmdJson(json) {}

std::string ICmd::asString()
{
    return _cmdJson[0].toStyledString();
    //throw std::runtime_error("ICmd: can't use asString(), please override this function");
}
