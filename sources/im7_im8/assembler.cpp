#include "assembler.h"
#include <shared_utils/assertion.h>
#include <list>
#include <regex>
#include "make_cmd.h"
#include "program.h"

namespace ayaz {

bool Assembler::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Assembler::preprocess(JSON &cmds)
{
    Program program = transform(cmds);

    std::map<std::string, std::string> strings;

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
    for (auto &str : strings) {
        const std::string &name = str.second;
        const std::string &content = str.first;
        program.push_back(makeCmd(name, {"db '" + content + "'"}));
    }

    cmds = transform(program);
}

void Assembler::postprocess(JSON &cmds)
{
    for (auto &cmd : cmds) {
        if (!cmd.isMember("args")) {
            continue;
        }

        for (auto &arg : cmd["args"]) {
            if (!arg.isString()) {
                continue;
            }
            const std::string argStr = arg.asString();

            static const std::regex isLocalVariable("l([0-9]+)");
            std::smatch match;
            if (std::regex_match(argStr, match, isLocalVariable) && match.size() == 2) {
                auto number = std::stoull(match[1].str());
                arg = "QWORD [rsp - " + std::to_string(8 * number) + "]";
            }
        }
    }
}

trm::Replacers Assembler::makeReplacers()
{
    return {};
}

std::string Assembler::getRules()
{
    return std::string("");
}

}
