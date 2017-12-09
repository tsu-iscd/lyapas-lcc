#pragma once

#include <CompositeCmd/Composites/CompositeCmd.h>
#include <CompositeCmd/ICmd.h>

class CmdFactory {
public:
    CompositeCmd::SPtrComposite createCompositeCmd(std::string, Json::Value);
    ICmd::SPtr createCmd(std::string, Json::Value);
};
