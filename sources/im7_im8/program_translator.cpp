#include "program_translator.h"
#include <shared_utils/assertion.h>
#include "make_cmd.h"

namespace ayaz {

void ProgramTranslator::process(Program &program_)
{
    program = &program_;
    for (current = std::begin(program_); current != std::end(program_); ++current) {
        JSON &cmd = *current;
        if (!cmd.isMember("type")) {
            continue;
        }
        std::string cmdType = cmd["type"].asString();

        if (cmdType == "label") {
            if (!cmd.isMember("name")) {
                continue;
            }

            std::string labelName = cmd["name"].asString();
            if (labelName == "main") {
                cmd = makeLabel("_start");
            }
        } else if (cmdType == "cmd") {
            auto found = handlers.find(cmd["cmd"].asString());
            if (found != std::end(handlers)) {
                Handler handler = found->second;
                (this->*handler)();
            }
        }
    }

    //
    // FIXME: добавлять только для main функции
    //
    program_.push_back(makeCmd("mov", {"rax", 60}));
    program_.push_back(makeCmd("mov", {"rdi", 0}));
    program_.push_back(makeCmd("syscall"));
}

auto ProgramTranslator::getStringStorage() -> const StringStorage &
{
    return strings;
}

void ProgramTranslator::handleStackAlloc()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    auto bytes = cmd["args"][0].asUInt64();

    cmd = makeCmd("sub", {"rsp", bytes * 8});
}

void ProgramTranslator::handleStackFree()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    auto bytes = cmd["args"][0].asUInt64();

    cmd = makeCmd("add", {"rsp", bytes * 8});
}

void ProgramTranslator::handleDefinitionString()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 2);
    const std::string &str = cmd["args"][0].asString();
    const JSON &var = cmd["args"][1];

    std::string alias = "__STR" + std::to_string(strings.size());
    strings.emplace(str, alias);
    alias = strings[str];

    cmd = makeCmd("mov", {var, alias});
}

void ProgramTranslator::handleWriteString()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 2);
    std::string var = cmd["args"][0].asString();
    JSON len = cmd["args"][1];

    Program inner{makeCmd("mov", {"rax", 1}),
                  makeCmd("mov", {"rdi", 1}),
                  makeCmd("mov", {"rsi", var}),
                  makeCmd("mov", {"rdx", len}),
                  makeCmd("syscall")};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

}
