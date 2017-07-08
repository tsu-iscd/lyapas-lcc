#pragma once

#include <CompositeCmd/ICmd.h>
#include <CompositeCmd/Composites/CompositeCmd.h>

class CmdFactory {
public:
    CompositeCmd::SPtrComposite createCompositeCmd(std::string, Json::Value);
    ICmd::SPtr createCmd(std::string, Json::Value);
};
