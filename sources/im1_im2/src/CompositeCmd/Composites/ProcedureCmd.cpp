#include "ProcedureCmd.h"

ProcedureCmd::ProcedureCmd(Json::Value processJson, ProcedureCmd::Type type)
    : CompositeCmd(processJson)
    , _type(type)
{
}

Json::Value ProcedureCmd::toJson()
{
    auto signature = buildSignature();

    if (_type == ProcedureCmd::Type::call) {
        return signature;
    } else if (_type == ProcedureCmd::Type::definition) {
        Json::Value result;

        result.append(signature);
        for (SPtr &child : _children) {
            auto paragraph = child->toJson();
            for (auto &cmd : paragraph) {
                result.append(cmd);
            }
        }

        return result;
    } else {
        throw std::logic_error("Unexpected case");
    }
}

Json::Value ProcedureCmd::buildSignature()
{
    Json::Value signature;
    signature[fieldName::type] = "cmd";
    signature[fieldName::cmd] = getTypeName();

    auto &signatureArgs = signature[fieldName::args];
    signatureArgs.append(_cmdJson[fieldName::name].asString());

    auto inArgs = _children.front()->toJson();
    for (auto &arg : inArgs) {
        signatureArgs.append(arg);
    }
    _children.pop_front();

    signatureArgs.append("/");

    auto outArgs = _children.front()->toJson();
    for (auto &arg : outArgs) {
        signatureArgs.append(arg);
    }
    _children.pop_front();

    return signature;
}

std::string ProcedureCmd::getTypeName()
{
    switch (_type) {
    case ProcedureCmd::Type::definition:
        return "definition";
    case ProcedureCmd::Type::call:
        return "call";
    default:
        throw std::logic_error("Unexpected case");
    }
}
