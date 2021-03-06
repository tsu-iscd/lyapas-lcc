#include "OperationCmd.h"

OperationCmd::OperationCmd(Json::Value processJson)
    : CompositeCmd(processJson)
{
}

Json::Value OperationCmd::toJson()
{
    Json::Value result;
    result[fieldName::type] = "cmd";
    std::string operationName = _cmdJson[fieldName::name].asString();

    if (_children.size() == 1) {
        auto child = _children.back()->toJson();
        result[fieldName::args] = child[fieldName::args];
    } else if (_children.size() >= 2) {
        for (SPtr &child : _children) {
            for (auto &arg : child->toArgumentFormat()) {
                result[fieldName::args].append(arg);
            }
        }
    }

    result[fieldName::cmd] = operationName;

    return result;
}
