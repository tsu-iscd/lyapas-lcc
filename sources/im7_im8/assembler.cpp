#include "assembler.h"
#include <regex>
#include <shared_utils/assertion.h>
#include <set>
#include "bss.h"
#include "builtin_functions.h"
#include "make_cmd.h"
#include "program_translator.h"
#include "program.h"
#include "registers.h"

namespace ayaz {
namespace {

// пример: l1
const std::regex isIrLocalVariable("l([0-9]+)");

// пример: p1
const std::regex isIrParamVariable("p([0-9]+)");

// пример: arg1
const std::regex isIrArgVariable("arg([0-9]+)");

// пример: 8byte ptr[index]
const std::regex isIrPtr(R"(([18]byte) ([^\[]+)\[([^\]]+)\])");

// пример: QWORD [ptr + offset]
const std::regex isAsmPtr(R"(([^ ]+) \[([^\]]+)\])");

}

bool Assembler::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Assembler::preprocess(JSON &cmds)
{
    Program program = transform(cmds);

    ProgramTranslator translator;
    translator.process(program);

    //
    // добавляем text секцию до программы
    //
    Program text{
        makeCmd("section", {".text"}),
        makeCmd("global", {"_start"}),
    };
    program.insert(std::begin(program), std::begin(text), std::end(text));

    //
    // добавляем data секцию после программы
    //
    program.push_back(makeCmd("section", {".data"}));
    const ProgramTranslator::StringStorage &strings = translator.getStringStorage();
    for (auto &str : strings) {
        const std::string &name = str.second;
        const std::string &content = str.first;
        program.push_back(makeCmd(name, {"db '" + content + "'"}));
    }

    //
    // добавляем bss секцию до программы
    //
    Program bssSection{
        makeCmd("section", {".bss"}),
        makeCmd(bss::seed + " resq 1"),
    };
    program.insert(std::begin(program), std::begin(bssSection), std::end(bssSection));

    //
    // добавляем builtin функции
    //
    appendBuiltinFunctions(program);

    cmds = transform(program);
}

void Assembler::postprocess(JSON &cmds)
{
    Program program = transform(cmds);

    processCmdsConstrains(program);
    processArgs(program);
    processMovWithDifferentArgsSizes(program);

    cmds = transform(program);
}

trm::Replacers Assembler::makeReplacers()
{
    return {};
}

std::string Assembler::getRules()
{
    return std::string("");
}

void Assembler::processArgs(Program &program)
{
    std::smatch match;
    auto isLocalOrParamVar = [&match](const std::string &var) {
        return std::regex_match(var, match, isIrLocalVariable) ||
               std::regex_match(var, match, isIrParamVariable);
    };

    auto localOrParamVarToPtr = [](const std::string &var) -> std::string {
        auto number = std::stoull(var);
        return "QWORD [" + regs::rsp + " + " + std::to_string(8 * number) + "]";
    };

    auto argVarToPtr = [](const std::string &var) -> std::string {
        auto number = std::stoull(var);
        return "QWORD [" + regs::rsp + " - " + std::to_string(8 * (1 + number)) + "]";
    };

    for (auto current = std::begin(program); current != std::end(program); ++current) {
        JSON &cmd = *current;
        if (!cmd.isMember("args")) {
            continue;
        }

        //
        // FIXME: использовать фильтры
        //
        for (auto &arg : cmd["args"]) {
            if (!arg.isString()) {
                continue;
            }
            const std::string argStr = arg.asString();

            if (isLocalOrParamVar(argStr)) {
                arg = localOrParamVarToPtr(match[1].str());
            }

            if (std::regex_match(argStr, match, isIrArgVariable)) {
                arg = argVarToPtr(match[1].str());
            }

            if (std::regex_match(argStr, match, isIrPtr)) {
                auto ptrType = match[1].str();
                auto ptr = match[2].str();
                auto index = match[3].str();

                std::string itemSize;
                if (ptrType == "1byte") {
                    ptrType = "BYTE";
                    itemSize = "1";
                } else if (ptrType == "8byte") {
                    ptrType = "QWORD";
                    itemSize = "8";
                } else {
                    LCC_FAIL("Unexpected case");
                }

                if (isLocalOrParamVar(ptr)) {
                    std::string var = localOrParamVarToPtr(match[1].str());
                    program.insert(current, makeCmd("mov", {regs::r15, var}));
                    ptr = regs::r15;
                }

                if (isLocalOrParamVar(index)) {
                    std::string var = localOrParamVarToPtr(match[1].str());
                    program.insert(current, makeCmd("mov", {regs::r14, var}));
                    index = regs::r14;
                }

                arg = ptrType + " [" + ptr + " + " + index + " * " + itemSize + "]";
            }
        }
    }
}

void Assembler::processCmdsConstrains(Program &program)
{
    //
    // трансляция инструкций, которые имеют
    // два обращения в память
    //
    for (auto current = std::begin(program); current != std::end(program); ++current) {
        JSON &cmd = *current;
        if (cmd["type"] != "cmd") {
            continue;
        }

        std::string cmdName = cmd["cmd"].asString();
        const thread_local std::set<std::string> cmds{
            "mov",
            "cmp",
            "add",
            "sub",
        };
        if (cmds.find(cmdName) == std::end(cmds)) {
            continue;
        }
        LCC_ASSERT(cmd["args"].size() == 2);


        size_t count = 0;

        auto countPtrAccess = [&count](JSON &arg) {
            std::string argStr = arg.asString();
            std::smatch match;
            if (std::regex_match(argStr, match, isAsmPtr) ||
                std::regex_match(argStr, match, isIrPtr) ||
                std::regex_match(argStr, match, isIrLocalVariable) ||
                std::regex_match(argStr, match, isIrParamVariable) ||
                std::regex_match(argStr, match, isIrArgVariable)) {
                ++count;
            }
        };

        JSON &arg1 = cmd["args"][0];
        countPtrAccess(arg1);

        JSON &arg2 = cmd["args"][1];
        countPtrAccess(arg2);

        if (count == 2) {
            program.insert(current, makeCmd("mov", {regs::rax, arg2}));
            cmd = makeCmd(cmdName, {arg1, regs::rax});
        }
    }
}

void Assembler::processMovWithDifferentArgsSizes(Program &program)
{
    for (auto current = std::begin(program); current != std::end(program); ++current) {
        JSON &cmd = *current;
        if (cmd["type"] != "cmd" || cmd["cmd"] != "mov") {
            continue;
        }

        JSON &args = cmd["args"];
        LCC_ASSERT(args.size() == 2);

        auto isByteAccessAndRax = [](const JSON &arg1, const JSON &arg2) -> bool {
            std::string arg1Str = arg1.asString();
            std::smatch match;
            bool isByteAccess = std::regex_match(arg1Str, match, isAsmPtr) && match[1].str() == "BYTE";
            bool isRax = arg2 == regs::rax;
            return isByteAccess && isRax;
        };

        if (isByteAccessAndRax(args[0], args[1])) {
            JSON &rax = args[1];
            rax = "al";
        } else if (isByteAccessAndRax(args[1], args[0])) {
            cmd["cmd"] = "movzx";
        }
    }
}

}
