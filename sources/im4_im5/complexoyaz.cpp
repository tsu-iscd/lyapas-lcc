#include "complexoyaz.h"
#include "replacers.h"

namespace cyaz {

bool Complexoyaz::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Complexoyaz::preprocess(JSON &cmds) {}

void Complexoyaz::postprocess(JSON &cmds) {}

trm::Replacers &Complexoyaz::getReplacers(const JSON &cmds)
{
    static trm::Replacers replacers;

    replacers.insert({"free_var", std::make_shared<cyaz::FreeVarReplacer>(cmds)});
    replacers.insert({"free_label", std::make_shared<cyaz::FreeLabelReplacer>(cmds)});

    auto complexStruct = [&](const trm::PatternStringInfo &patternStringInfo) -> std::string {
        return "<complex" + patternStringInfo.getGroupAsString() + ">_struct";
    };
    replacers.insert({"complex_struct",
                      std::make_shared<cyaz::FunctionalReplacer>(complexStruct)});

    auto complexCardinality = [&](const trm::PatternStringInfo &patternStringInfo) -> std::string {
        return "8byte <complex" + patternStringInfo.getGroupAsString() + ">_struct[0]";
    };
    replacers.insert({"complex_cardinality",
                      std::make_shared<cyaz::FunctionalReplacer>(complexCardinality)});

    auto complexCapacity = [&](const trm::PatternStringInfo &patternStringInfo) -> std::string {
        return "8byte <complex" + patternStringInfo.getGroupAsString() + ">_struct[1]";
    };
    replacers.insert({"complex_capacity",
                      std::make_shared<cyaz::FunctionalReplacer>(complexCapacity)});

    auto complexBuffer = [&](const trm::PatternStringInfo &patternStringInfo) -> std::string {
        return "8byte <complex" + patternStringInfo.getGroupAsString() + ">_struct[2]";
    };
    replacers.insert({"complex_buffer",
                      std::make_shared<cyaz::FunctionalReplacer>(complexBuffer)});

    auto complexBufferOpt = [&](const trm::PatternStringInfo &patternStringInfo) -> std::string {
        return "<complex" + patternStringInfo.getGroupAsString() + ">_buffer";
    };
    replacers.insert({"complex_buffer_opt",
                      std::make_shared<cyaz::FunctionalReplacer>(complexBufferOpt)});

    auto complexCell = [&](const trm::PatternStringInfo &patternStringInfo) -> std::string {
        const std::string &group = patternStringInfo.getGroupAsString();
        const auto &param = patternStringInfo.getParam();

        if (!param) {
            throw std::runtime_error("Не указан параметр");
        }

        auto p = patternStringInfo.getStringMap().find("complex" + group);
        if (p == patternStringInfo.getStringMap().end()) {
            throw std::runtime_error("У транслируемой команды нет аргумента <complex>");
        }
        std::string byteSize = (p->second[0] == 'L' ? "8byte" : "1byte");

        return byteSize + " <complex" + group + ">_buffer[" + *param + "]";
    };
    replacers.insert({"complex_cell",
                      std::make_shared<cyaz::FunctionalReplacer>(complexCell)});

    auto stringLen = [&](const trm::PatternStringInfo &patternStringInfo) -> std::string {
        auto p = patternStringInfo.getStringMap().find("string" + patternStringInfo.getGroupAsString());
        if (p == patternStringInfo.getStringMap().end()) {
            throw std::runtime_error("У транслируемой команды нет аргумента <string>");
        }

        return std::to_string(p->second.size());
    };
    replacers.insert({"string_len",
                      std::make_shared<cyaz::FunctionalReplacer>(stringLen)});

    return replacers;
}

std::string Complexoyaz::getRules()
{
    return std::string(
#include "rules.txt"
    );
}

}  // namespace cyaz
