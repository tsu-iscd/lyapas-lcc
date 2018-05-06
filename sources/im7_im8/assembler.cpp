#include "assembler.h"
#include <regex>
#include "program_translator.h"
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
    for (auto &cmd : cmds) {
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
