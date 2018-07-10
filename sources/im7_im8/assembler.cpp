#include "assembler.h"
#include <regex>
#include <shared_utils/assertion.h>
#include "make_cmd.h"
#include "program_translator.h"
#include "program.h"
#include "registers.h"

namespace ayaz {

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

    cmds = transform(program);
}

void Assembler::postprocess(JSON &cmds)
{
    Program program = transform(cmds);

    processArgs(program);
    processCmdsConstrains(program);

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
    auto localVarToPtr = [](const std::string &var) -> std::string {
        auto number = std::stoull(var);
        return "QWORD [" + regs::rsp + " - " + std::to_string(8 * number) + "]";
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

            // пример: l1
            static const std::regex isLocalVariable("l([0-9]+)");
            std::smatch match;
            if (std::regex_match(argStr, match, isLocalVariable) && match.size() == 2) {
                arg = localVarToPtr(match[1].str());
            }

            // пример: 8byte ptr[index]
            static const std::regex isPtr(R"(([18]byte) ([^\[]+)\[([^\]]+)\])");
            if (std::regex_match(argStr, match, isPtr)) {
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

                if (std::regex_match(ptr, match, isLocalVariable)) {
                    std::string var = localVarToPtr(match[1].str());
                    program.insert(current, makeCmd("mov", {regs::r15, var}));
                    ptr = regs::r15;
                }

                if (std::regex_match(index, match, isLocalVariable)) {
                    std::string var = localVarToPtr(match[1].str());
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
    for (auto current = std::begin(program); current != std::end(program); ++current) {
        JSON &cmd = *current;
        if (cmd["type"] != "cmd" || cmd["cmd"] != "mov") {
            continue;
        }

        size_t count = 0;

        // пример: QWORD [ptr + offset]
        auto isPtr = [&count](JSON &arg) {
            static const std::regex isPtrRegex(R"(([^ ]+) \[([^\]]+)\])");
            std::smatch match;

            std::string argStr = arg.asString();
            if (std::regex_match(argStr, match, isPtrRegex)) {
                ++count;
            }
        };

        JSON &arg1 = cmd["args"][0];
        isPtr(arg1);

        JSON &arg2 = cmd["args"][1];
        isPtr(arg2);

        if (count == 2) {
            program.insert(current, makeCmd("mov", {regs::rax, arg2}));
            cmd = makeCmd("mov", {arg1, regs::rax});
        }
    }
}

}
