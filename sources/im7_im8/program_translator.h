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
    void handleReadString();
    void handleWriteString();
    void handleAlloc();
    void handleMove();
    void handleAdd();
    void handleMul();
    void handleMod();
    void handleDiv();
    void handleCompare();
    void handleJump();
    void handleError();
    void handleSetMin();
    void handleSetMax();

    void handleGetTime();
    void handleSetRandom();
    void handleGetRandom();

    void handleGetNumberLeastOne();
    void handleWeight();

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
        {"read_string", &ProgramTranslator::handleReadString},
        {"write_string", &ProgramTranslator::handleWriteString},
        {"alloc", &ProgramTranslator::handleAlloc},
        {"alloc_at_least", &ProgramTranslator::handleAlloc},
        {"move", &ProgramTranslator::handleMove},
        {"add", &ProgramTranslator::handleAdd},
        {"mul", &ProgramTranslator::handleMul},
        {"mod", &ProgramTranslator::handleMod},
        {"div", &ProgramTranslator::handleDiv},
        {"compare", &ProgramTranslator::handleCompare},

        {"jump", &ProgramTranslator::handleJump},
        {"jump_lt", &ProgramTranslator::handleJump},
        {"jump_leq", &ProgramTranslator::handleJump},
        {"jump_gt", &ProgramTranslator::handleJump},
        {"jump_geq", &ProgramTranslator::handleJump},
        {"jump_eq", &ProgramTranslator::handleJump},
        {"jump_neq", &ProgramTranslator::handleJump},

        {"error", &ProgramTranslator::handleError},

        {"set_min", &ProgramTranslator::handleSetMin},
        {"set_max", &ProgramTranslator::handleSetMax},

        {"get_time", &ProgramTranslator::handleGetTime},
        {"set_random", &ProgramTranslator::handleSetRandom},
        {"get_random", &ProgramTranslator::handleGetRandom},

        {"get_number_least_1", &ProgramTranslator::handleGetNumberLeastOne},
        {"weight", &ProgramTranslator::handleWeight},
    };
}

}
