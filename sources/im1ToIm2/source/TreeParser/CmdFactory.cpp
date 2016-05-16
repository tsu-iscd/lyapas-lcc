//
// Created by s15-22 on 07.11.15.
//

#include <iostream>
#include "CmdFactory.h"
#include "../CompositeCmd/Compisities/ProcedureCmd.h"
#include "../CompositeCmd/Compisities/StabCompositeCmd.h"
#include "../CompositeCmd/Compisities/ParagraphCmd.h"
#include "../CompositeCmd/Compisities/OperationCmd.h"
#include "../CompositeCmd/Compisities/ComplexCmd.h"
#include "../CompositeCmd/Compisities/PrefaceCmd.h"
#include "../CompositeCmd/Compisities/ProgramCmd.h"
#include "../CompositeCmd/Compisities/ArgsCmd.h"
#include "../CompositeCmd/ICmd.h"
#include "../CompositeCmd/Leafs/StabLeafCmd.h"
#include "../CompositeCmd/Leafs/VariableCmd.h"
#include "../CompositeCmd/Leafs/ConstantCmd.h"
#include "../CompositeCmd/Leafs/StringCmd.h"
#include "../CompositeCmd/Leafs/AsmCmd.h"

CompositeCmd::SPtrComposite CmdFactory::createCompositeCmd(std::string type, Json::Value json)
{
    if("program" == type)
    {
        return CompositeCmd::SPtrComposite(new ProgramCmd(json));
    }
    else if("in_args" == type || "out_args" == type)
    {
        return CompositeCmd::SPtrComposite(new ArgsCmd(json));
    }
    else if("procedure" == type)
    {
        return CompositeCmd::SPtrComposite(new ProcedureCmd(json, ProcedureCmd::Type::definition));
    }
    else if("call" == type)
    {
        return CompositeCmd::SPtrComposite(new ProcedureCmd(json, ProcedureCmd::Type::call));
    }
    else if("preface" == type)
    {
        return CompositeCmd::SPtrComposite(new PrefaceCmd(json));
    }
    else if("par" == type)
    {
        return CompositeCmd::SPtrComposite(new ParagraphCmd(json));
    }
    else if("operation" == type)
    {
        return CompositeCmd::SPtrComposite(new OperationCmd(json));
    }
    else if(type.find("complex") != std::string::npos)
    //else if("complex" == type)
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
    else if("string" == type)
    {
        return StringCmd::SPtr(new StringCmd(json));
    }
    else if("asm" == type)
    {
        return AsmCmd::SPtr(new AsmCmd(json));
    }
    else
    {
        return ICmd::SPtr(new StabLeafCmd(json));
    }
}

