#include "program_translator.h"
#include <shared_utils/assertion.h>
#include "make_cmd.h"

namespace ayaz {

void ProgramTranslator::process(Program &program)
{
    for (auto current = std::begin(program); current != std::end(program); ++current) {
        JSON &cmd = *current;

        if (cmd.isMember("type") && cmd["type"] == "label") {
            if (!cmd.isMember("name")) {
                continue;
            }

            std::string labelName = cmd["name"].asString();
            if (labelName == "main") {
                cmd = makeLabel("_start");
            }
            continue;
        }

        if (!cmd.isMember("type") || cmd["type"] != "cmd") {
            continue;
        }

        std::string cmdName = cmd["cmd"].asString();
        if (cmdName == "stack_alloc") {
            LCC_ASSERT(cmd["args"].size() == 1);
            auto bytes = cmd["args"][0].asUInt64();

            cmd = makeCmd("sub", {"rsp", bytes * 8});
            continue;
        }

        if (cmdName == "stack_free") {
            LCC_ASSERT(cmd["args"].size() == 1);
            auto bytes = cmd["args"][0].asUInt64();

            cmd = makeCmd("add", {"rsp", bytes * 8});
            continue;
        }

        if (cmdName == "definition_string") {
            LCC_ASSERT(cmd["args"].size() == 2);
            const std::string &str = cmd["args"][0].asString();
            const JSON &var = cmd["args"][1];

            std::string alias = "__STR" + std::to_string(strings.size());
            strings.emplace(str, alias);
            alias = strings[str];

            cmd = makeCmd("mov", {var, alias});
            continue;
        }

        if (cmdName == "write_string") {
            LCC_ASSERT(cmd["args"].size() == 2);
            std::string var = cmd["args"][0].asString();
            JSON len = cmd["args"][1];

            Program inner{makeCmd("mov", {"rax", 1}),
                          makeCmd("mov", {"rdi", 1}),
                          makeCmd("mov", {"rsi", var}),
                          makeCmd("mov", {"rdx", len}),
                          makeCmd("syscall")};

            current = program.erase(current);
            program.insert(current, std::begin(inner), std::end(inner));
            --current;
            continue;
        }
    }

    //
    // FIXME: добавлять только для main функции
    //
    program.push_back(makeCmd("mov", {"rax", 60}));
    program.push_back(makeCmd("mov", {"rdi", 0}));
    program.push_back(makeCmd("syscall"));
}

auto ProgramTranslator::getStringStorage() -> const StringStorage &
{
    return strings;
}

}
