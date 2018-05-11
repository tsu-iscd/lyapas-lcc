#include "program_translator.h"
#include <unordered_map>
#include <shared_utils/assertion.h>
#include <sys/mman.h>
#include "label_fix.h"
#include "make_cmd.h"
#include "registers.h"

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
            // FIXME: временное решение
            tofix::fixLabel(cmd);

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
    program_.push_back(makeCmd("mov", {regs::rax, 60}));
    program_.push_back(makeCmd("mov", {regs::rdi, 0}));
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

    cmd = makeCmd("sub", {regs::rsp, bytes * 8});
}

void ProgramTranslator::handleStackFree()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    auto bytes = cmd["args"][0].asUInt64();

    cmd = makeCmd("add", {regs::rsp, bytes * 8});
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

    Program inner{makeCmd("mov", {regs::rax, 1}),
                  makeCmd("mov", {regs::rdi, 1}),
                  makeCmd("mov", {regs::rsi, var}),
                  makeCmd("mov", {regs::rdx, len}),
                  makeCmd("syscall")};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleAlloc()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 2);
    JSON size = cmd["args"][0];
    JSON dstPtr = cmd["args"][1];

    // используем mmap
    Program inner{makeCmd("mov", {regs::rax, 9}),
                  makeCmd("mov", {regs::rdi, 0}),
                  makeCmd("mov", {regs::rsi, size}),
                  makeCmd("mov", {regs::rdx, PROT_READ | PROT_WRITE}),
                  makeCmd("mov", {regs::r10, MAP_PRIVATE | MAP_ANONYMOUS}),
                  makeCmd("mov", {regs::r8, -1}),
                  makeCmd("mov", {regs::r9, 0}),
                  makeCmd("syscall"),
                  makeCmd("mov", {dstPtr, regs::rax})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleMove()
{
    JSON &cmd = *current;
    cmd["cmd"] = "mov";
}

void ProgramTranslator::handleAdd()
{
    JSON &cmd = *current;
    cmd["cmd"] = "add";
}

void ProgramTranslator::handleMul()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 2);
    JSON arg1 = cmd["args"][0];
    JSON arg2 = cmd["args"][1];

    program->insert(current, makeCmd("mov", {regs::rax, arg2}));
    cmd = makeCmd("mul", {arg1});
    program->insert(std::next(current), makeCmd("mov", {arg1, regs::rax}));
}

void ProgramTranslator::handleMod()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 3);
    JSON arg1 = cmd["args"][0];
    JSON arg2 = cmd["args"][1];
    JSON arg3 = cmd["args"][2];

    Program inner{makeCmd("mov", {regs::rdx, 0}),
                  makeCmd("mov", {regs::rax, arg1}),
                  makeCmd("div", {arg2}),
                  makeCmd("mov", {arg3, regs::rax}),
                  makeCmd("mov", {arg1, regs::rdx})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleDiv()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 3);
    JSON arg1 = cmd["args"][0];
    JSON arg2 = cmd["args"][1];
    JSON arg3 = cmd["args"][2];

    Program inner{makeCmd("mov", {regs::rdx, 0}),
                  makeCmd("mov", {regs::rax, arg1}),
                  makeCmd("div", {arg2}),
                  makeCmd("mov", {arg1, regs::rax}),
                  makeCmd("mov", {arg3, regs::rdx})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleCompare()
{
    JSON &cmd = *current;
    cmd["cmd"] = "cmp";
}

void ProgramTranslator::handleJump()
{
    JSON &cmd = *current;
    std::string cmdName = cmd["cmd"].asString();

    const thread_local std::unordered_map<std::string, std::string> map{
        {"jump", "jmp"},
        {"jump_lt", "jl"},
        {"jump_leq", "jle"},
        {"jump_gt", "jg"},
        {"jump_geq", "jge"},
        {"jump_eq", "je"},
        {"jump_neq", "jne"},
    };

    auto found = map.find(cmdName);
    LCC_ASSERT(found != std::end(map));
    cmd["cmd"] = found->second;

    // FIXME: временное решение
    tofix::fixJump(cmd);
}

void ProgramTranslator::handleError()
{
    //
    // TODO: выводить ошибку на экран
    //
    JSON &cmd = *current;

    Program inner{makeCmd("mov", {regs::rax, 60}),
                  makeCmd("mov", {regs::rdi, 1}),
                  makeCmd("syscall")};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

}
