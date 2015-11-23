//
// Created by s15-22 on 07.11.15.
//

#include "CmdFactory.h"
#include "../CompositeCmd/Compisities/ProcessCmd.h"
#include "../CompositeCmd/Compisities/StabCompositeCmd.h"
#include "../CompositeCmd/Leafs/StabLeafCmd.h"
#include "../CompositeCmd/Compisities/ParagraphCmd.h"
#include "../CompositeCmd/Compisities/OperationCmd.h"
#include "../CompositeCmd/Leafs/VariableCmd.h"
#include "../CompositeCmd/Leafs/ConstantCmd.h"
#include "../CompositeCmd/Compisities/ComplexCmd.h"

CompositeCmd::SPtrComposite CmdFactory::createCompositeCmd(std::string type, Json::Value json)
{
    if("proc" == type)
    {
        return CompositeCmd::SPtrComposite(new ProcessCmd(json));
    }
    else if("preface" == type)
    {
        return CompositeCmd::SPtrComposite(new ParagraphCmd(json));
    }
    else if("par" == type)
    {
        return CompositeCmd::SPtrComposite(new ParagraphCmd(json));
    }
    else if("op" == type)
    {
        return CompositeCmd::SPtrComposite(new OperationCmd(json));
    }
    else if("complex" == type)
    {
        return CompositeCmd::SPtrComposite(new ComplexCmd(json));
    }
    else
    {
        return NULL;
    }
}

ICmd::SPtr CmdFactory::createCmd(std::string type, Json::Value json)
{
    if("var" == type)
    {
        return VariableCmd::SPtr(new VariableCmd(json));
    }
    else if("const" == type)
    {
        return ConstantCmd::SPtr(new ConstantCmd(json));
    }
    else
    {
        return ICmd::SPtr(new StabLeafCmd(json));
    }
}

