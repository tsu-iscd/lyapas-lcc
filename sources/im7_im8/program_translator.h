#pragma once

#include "program.h"
#include <map>
#include <string>

namespace ayaz {

class ProgramTranslator {
public:
    using StringStorage = std::map<std::string, std::string>;

    void process(Program &program);
    const StringStorage &getStringStorage();

private:
    void handleStackAlloc();
    void handleStackFree();
    void handleDefinitionString();
    void handleWriteString();

    using Handler = void (ProgramTranslator::*)();
    using Handlers = std::map<std::string, Handler>;

    Handlers makeHandlers();
    Handlers handlers = makeHandlers();

    StringStorage strings;

    Program *program = nullptr;
    Program::iterator current;
};

inline auto ProgramTranslator::makeHandlers() -> Handlers
{
    return {
        {"stack_alloc", &ProgramTranslator::handleStackAlloc},
        {"stack_free", &ProgramTranslator::handleStackFree},
        {"definition_string", &ProgramTranslator::handleDefinitionString},
        {"write_string", &ProgramTranslator::handleWriteString},
    };
}

}
