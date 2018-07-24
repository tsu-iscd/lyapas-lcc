#include "program_translator.h"
#include <unordered_map>
#include <shared_utils/assertion.h>
#include <sys/mman.h>
#include "bss.h"
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

            if (!cmd.isMember("name")) {
                continue;
            }

            std::string labelName = cmd["name"].asString();
            if (labelName == "main") {
                cmd = makeLabel("_start");
            }
        } else if (cmdType == "cmd") {
            std::string cmdName = cmd["cmd"].asString();
            auto found = handlers.find(cmdName);
            if (found != std::end(handlers)) {
                Handler handler = found->second;
                (this->*handler)();
            } else {
                // FIXME: раскомментировать позже
                //std::string error = "invalid cmd: " + cmdName;
                //LCC_FAIL(error.c_str());
                //
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

void ProgramTranslator::handleReadString()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 3);
    std::string addr = cmd["args"][0].asString();
    JSON len = cmd["args"][1];
    JSON readLen = cmd["args"][2];

    Program inner{makeCmd("mov", {regs::rax, 0}),
                  makeCmd("mov", {regs::rdi, 1}),
                  makeCmd("mov", {regs::rsi, addr}),
                  makeCmd("mov", {regs::rdx, len}),
                  makeCmd("syscall"),
                  makeCmd("mov", {readLen, regs::rax})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
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

    //
    // используем mmap
    //
    // выделяется на 8 байт больше для сохранения размера выделенной памяти
    //
    Program inner{makeCmd("mov", {regs::rax, 9}),
                  makeCmd("mov", {regs::rdi, 0}),
                  makeCmd("mov", {regs::rsi, size}),
                  makeCmd("add", {regs::rsi, 8}),
                  makeCmd("mov", {regs::rdx, PROT_READ | PROT_WRITE}),
                  makeCmd("mov", {regs::r10, MAP_PRIVATE | MAP_ANONYMOUS}),
                  makeCmd("mov", {regs::r8, -1}),
                  makeCmd("mov", {regs::r9, 0}),
                  makeCmd("syscall"),
                  makeCmd("mov", {regs::rdi, regs::rax}),
                  makeCmd("mov", {"QWORD [" + regs::rdi + "]", size}),
                  makeCmd("add", {"QWORD [" + regs::rdi + "]", 8}),
                  makeCmd("add", {regs::rdi, 8}),
                  makeCmd("mov", {dstPtr, regs::rdi})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleRealloc()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 2);
    JSON len = cmd["args"][0];
    JSON addr = cmd["args"][1];

    Program inner{// rsi -- dst
                  makeCmd("alloc", {len, regs::rsi}),
                  // rdi -- src
                  makeCmd("mov", {regs::rdi, addr}),
                  // rdx -- srcLen
                  makeCmd("mov", {regs::rdx, regs::rdi}),
                  makeCmd("sub", {regs::rdx, 8}),
                  makeCmd("mov", {regs::rdx, "QWORD [" + regs::rdx + "]"}),
                  // выбираем минимум из len и srcLen
                  makeCmd("cmp", {regs::rdx, len}),
                  makeCmd("cmova", {regs::rdx, len}),
                  makeCmd("call", {"__copy"}),
                  makeCmd("dealloc", {addr})};

    current = program->erase(current);
    program->insert(current--, std::begin(inner), std::end(inner));
}

void ProgramTranslator::handleDealloc()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    JSON addr = cmd["args"][0];

    //
    // используем munmap
    //
    Program inner{makeCmd("mov", {regs::rax, 11}),
                  makeCmd("mov", {regs::rdi, addr}),
                  makeCmd("sub", {regs::rdi, 8}),
                  makeCmd("mov", {regs::rsi, "QWORD [" + regs::rdi + "]"}),
                  makeCmd("syscall")};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleMove()
{
    JSON &cmd = *current;
    cmd["cmd"] = "mov";
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

void ProgramTranslator::handleDdiv()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 3);
    JSON lo = cmd["args"][0];
    JSON denominator = cmd["args"][1];
    JSON hi = cmd["args"][2];

    JSON quotient = cmd["args"][0];
    JSON remainder = cmd["args"][2];

    Program inner{makeCmd("mov", {regs::rdi, lo}),
                  makeCmd("mov", {regs::rsi, hi}),
                  makeCmd("mov", {regs::rdx, denominator}),
                  makeCmd("call", {"__div_128_64"}),
                  makeCmd("mov", {quotient, regs::rdi}),
                  makeCmd("mov", {remainder, regs::rsi})};

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
        {"jump_lt", "jb"},
        {"jump_leq", "jbe"},
        {"jump_gt", "ja"},
        {"jump_geq", "jae"},
        {"jump_eq", "je"},
        {"jump_neq", "jne"},
    };

    auto found = map.find(cmdName);
    LCC_ASSERT(found != std::end(map));
    cmd["cmd"] = found->second;

    // FIXME: временное решение
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

void ProgramTranslator::handleSetMin()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    cmd["cmd"] = "mov";
    cmd["args"].append(0);
}

void ProgramTranslator::handleSetMax()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    cmd["cmd"] = "mov";
    cmd["args"].append(0xffff'ffff'ffff'ffffull);
}

void ProgramTranslator::handleGetTime()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    JSON arg1 = cmd["args"][0];

    Program inner{makeCmd("mov", {regs::rax, 201}),
                  makeCmd("mov", {regs::rdi, 0}),
                  makeCmd("syscall"),
                  makeCmd("mov", {arg1, regs::rax})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleSetRandom()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    JSON arg1 = cmd["args"][0];

    Program inner{makeCmd("mov", {regs::rax, arg1}),
                  makeCmd("mov", {"[" + bss::seed + "]", regs::rax})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleGetRandom()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    JSON arg1 = cmd["args"][0];

    Program inner{makeCmd("mov", {regs::rcx, "[" + bss::seed + "]"}),
                  makeCmd("mov", {regs::rdx, regs::rcx}),
                  makeCmd("shr", {regs::rdx, 12}),
                  makeCmd("xor", {regs::rcx, regs::rdx}),
                  makeCmd("mov", {regs::rdx, regs::rcx}),
                  makeCmd("shl", {regs::rdx, 25}),
                  makeCmd("xor", {regs::rcx, regs::rdx}),
                  makeCmd("mov", {regs::rdx, regs::rcx}),
                  makeCmd("shr", {regs::rdx, 27}),
                  makeCmd("xor", {regs::rcx, regs::rdx}),
                  makeCmd("mov", {regs::rax, 2685821657736338717ull}),
                  makeCmd("mul", {regs::rcx}),
                  makeCmd("mov", {"[" + bss::seed + "]", regs::rcx}),
                  makeCmd("mov", {arg1, regs::rax})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleGetNumberLeastOne()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    JSON arg = cmd["args"][0];

    Program inner{makeCmd("mov", {regs::rdi, arg}),
                  makeCmd("call", {"__tzcnt"}),
                  makeCmd("mov", {arg, regs::rax})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleWeight()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    JSON arg = cmd["args"][0];

    Program inner{makeCmd("mov", {regs::rdi, arg}),
                  makeCmd("call", {"__weight"}),
                  makeCmd("mov", {arg, regs::rax})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleEnumeration()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 2);
    JSON value = cmd["args"][0];
    JSON indexOfRightOne = cmd["args"][1];

    Program inner{makeCmd("mov", {regs::rdi, value}),
                  makeCmd("call", {"__enumeration"}),
                  makeCmd("mov", {indexOfRightOne, regs::rax}),
                  makeCmd("mov", {value, regs::rbx})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

void ProgramTranslator::handleAsm()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
    JSON asmInstruction = cmd["args"][0];

    cmd = makeCmd(asmInstruction.asString());
}

void ProgramTranslator::handleUnaryOp()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 1);
}

void ProgramTranslator::handleBinaryOp()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 2);
}

void ProgramTranslator::handleShift()
{
    JSON &cmd = *current;
    LCC_ASSERT(cmd["args"].size() == 2);
    JSON value = cmd["args"][0];
    JSON shift = cmd["args"][1];

    const thread_local std::unordered_map<std::string, std::string> map{
        {"left_shift", "shl"},
        {"right_shift", "shr"},
    };
    std::string cmdName = cmd["cmd"].asString();
    auto found = map.find(cmdName);
    LCC_ASSERT(found != std::end(map));
    cmdName = found->second;

    Program inner{makeCmd("mov", {regs::rcx, shift}),
                  makeCmd(cmdName, {value, regs::cl})};

    current = program->erase(current);
    program->insert(current, std::begin(inner), std::end(inner));
    --current;
}

}
