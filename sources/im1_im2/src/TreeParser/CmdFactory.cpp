#include <iostream>
#include "CmdFactory.h"
#include "../CompositeCmd/Composites/ProcedureCmd.h"
#include "../CompositeCmd/Composites/ParagraphCmd.h"
#include "../CompositeCmd/Composites/OperationCmd.h"
#include "../CompositeCmd/Composites/ComplexCmd.h"
#include "../CompositeCmd/Composites/PrefaceCmd.h"
#include "../CompositeCmd/Composites/ProgramCmd.h"
#include "../CompositeCmd/Composites/ArgsCmd.h"

#include "../CompositeCmd/Leafs/VariableCmd.h"
#include "../CompositeCmd/Leafs/ConstantCmd.h"
#include "../CompositeCmd/Leafs/StringCmd.h"
#include "../CompositeCmd/Leafs/AsmCmd.h"

CompositeCmd::SPtrComposite CmdFactory::createCompositeCmd(std::string type, Json::Value json)
{
    if("program" == type) {
        return CompositeCmd::SPtrComposite(new ProgramCmd(json));
    } else if("in_args" == type || "out_args" == type) {
        return CompositeCmd::SPtrComposite(new ArgsCmd(json));
    } else if("procedure" == type) {
        return CompositeCmd::SPtrComposite(new ProcedureCmd(json, ProcedureCmd::Type::definition));
    } else if("call" == type) {
        return CompositeCmd::SPtrComposite(new ProcedureCmd(json, ProcedureCmd::Type::call));
    } else if("preface" == type) {
        return CompositeCmd::SPtrComposite(new PrefaceCmd(json));
    } else if("par" == type) {
        return CompositeCmd::SPtrComposite(new ParagraphCmd(json));
    } else if("operation" == type) {
        return CompositeCmd::SPtrComposite(new OperationCmd(json));
    } else if(type.find("complex") != std::string::npos) {
        return CompositeCmd::SPtrComposite(new ComplexCmd(json));
    } else {
        return nullptr;
    }
}

ICmd::SPtr CmdFactory::createCmd(std::string type, Json::Value json)
{
    if("var" == type) {
        return VariableCmd::SPtr(new VariableCmd(json));
    } else if("const" == type) {
        return ConstantCmd::SPtr(new ConstantCmd(json));
    } else if("string" == type) {
        return StringCmd::SPtr(new StringCmd(json));
    } else if("asm" == type) {
        return AsmCmd::SPtr(new AsmCmd(json));
    } else {
        return nullptr;
    }
}
