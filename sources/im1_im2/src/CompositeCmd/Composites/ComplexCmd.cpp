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
        throw std::runtime_error("Invalid type of complex: " + complexType);
    }

    if (_cmdJson.isMember(fieldName::number)) {
        complexType += _cmdJson[fieldName::number].asString();
    }


    if (_children.size() == 1) {
        Json::Value child = _children.back()->toJson();
        Json::Value &args = child[fieldName::args];
        if (args.size() != 1) {
            throw std::runtime_error("Invalid count of child's args. Expected 1, actually " +
                                     std::to_string(args.size()));
        }

        complexType += "[";
        complexType += args[0].asString();
        complexType += "]";
    }

    Json::Value result;
    result[fieldName::args].append(complexType);
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
