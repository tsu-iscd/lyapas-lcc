#include "ComplexCmd.h"

ComplexCmd::ComplexCmd(Json::Value processJson)
    : CompositeCmd(processJson)
{
    nameMap.emplace("symbol_complex", "F");
    nameMap.emplace("logic_complex", "L");
    nameMap.emplace("global_complex_1", "G1");
    nameMap.emplace("global_complex_4", "G4");
    nameMap.emplace("complex_cardinality", "Q");
    nameMap.emplace("complex_capacity", "S");
}

Json::Value ComplexCmd::toJson()
{
    if (_children.size() > 1) {
        throw std::runtime_error("Complex can't have more one child; correct children = 1 or 0");
    }

    std::string complexName = getComplexName(_cmdJson);

    if (_children.size() == 1) {
        Json::Value child = _children.back()->toJson();
        Json::Value &args = child[fieldName::args];
        if (args.size() != 1) {
            throw std::runtime_error("Invalid count of child's args. Expected 1, actually " +
                                     std::to_string(args.size()));
        }

        complexName += '[';
        complexName += args[0].asString();
        complexName += ']';
    }

    Json::Value result;
    result[fieldName::args].append(complexName);
    return result;
}

Json::Value ComplexCmd::toArgumentFormat()
{
    if (_children.size() != 0) {
        throw std::runtime_error("Complex can't have any children when it contains in arguments of procedure");
    }

    Json::Value result;
    result.append(getComplexName(_cmdJson));
    return result;
}

std::string ComplexCmd::getComplexName(Json::Value &complexJson) const
{
    std::string complexName = _cmdJson[fieldName::type].asString();
    auto it = nameMap.find(complexName);
    if (it != nameMap.end()) {
        complexName = it->second;
    } else {
        throw std::runtime_error("Invalid type of complex: " + complexName);
    }

    if (_cmdJson.isMember(fieldName::number)) {
        complexName += _cmdJson[fieldName::number].asString();
    }

    return complexName;
}
