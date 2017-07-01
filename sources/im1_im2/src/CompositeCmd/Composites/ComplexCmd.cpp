#include "ComplexCmd.h"

ComplexCmd::ComplexCmd(Json::Value processJson) :
    CompositeCmd(processJson)
{
    nameMap.emplace("symbol_complex", "F");
    nameMap.emplace("logic_complex", "L");
    nameMap.emplace("global_complex_1", "G1");
    nameMap.emplace("global_complex_4", "G4");
}

Json::Value ComplexCmd::toJson()
{
    if (_children.size() > 1) {
        throw std::runtime_error("Complex can't have more one child; correct children = 1 or 0");
    }

    std::string complexType = _cmdJson[fieldName::type].asString();
    auto it = nameMap.find(complexType);
    if (it != nameMap.end()) {
        complexType = it->second;
    } else {
        throw std::runtime_error("Invalid type of complex");
    }

    if (_cmdJson.isMember(fieldName::number)) {
        complexType += _cmdJson[fieldName::number].asString();
    }

    Json::Value result;
    result[fieldName::args].append(complexType);

    if (_children.size() == 1) {
        auto child = _children.back()->toJson();
        for (auto &arg : child[fieldName::args]) {
            result[fieldName::args].append(arg);
        }
    }

    return result;
}

Json::Value ComplexCmd::toArgumentFormat()
{
    if(_children.size() != 0) throw std::runtime_error("Complex can't have any children when it contains in arguments of procedure");

    Json::Value result;
    result.append(_cmdJson[fieldName::type].asString());
    result.append(_cmdJson[fieldName::number]);
    return result;
}
