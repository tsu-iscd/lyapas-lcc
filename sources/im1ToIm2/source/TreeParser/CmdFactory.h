//
// Created by s15-22 on 07.11.15.
//

#ifndef IM1TOIM2_CMDFACTORY_H
#define IM1TOIM2_CMDFACTORY_H


#include "../CompositeCmd/ICmd.h"
#include "../CompositeCmd/CompositeCmd.h"

class CmdFactory {
public:
    CompositeCmd::SPtrComposite createCompositeCmd(std::string, Json::Value);
    ICmd::SPtr createCmd(std::string, Json::Value);
};


#endif //IM1TOIM2_CMDFACTORY_H
