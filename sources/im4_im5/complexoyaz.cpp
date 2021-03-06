#include "complexoyaz.h"
#include <memory>
#include <regex>
#include <stdexcept>
#include <translation_module/args_range.h>
#include <translation_module/replacers.h>
#include "functional_replacers.h"

namespace cyaz {

namespace {

const std::string &tryExtract(const trm::PatternStringInfo &info, const std::string &name)
{
    const std::string &key = name + info.getGroupAsString();
    auto p = info.getStringMap().find(key);
    if (p == info.getStringMap().end()) {
        throw std::runtime_error("У транслируемой команды нет аргумента '" + key + "'");
    }
    return p->second;
}

std::string calculateElementSize(const std::string &complex)
{
    if (complex.empty()) {
        throw std::runtime_error("Пустое имя комплекса");
    }

    char firstChar = complex[0];
    if (firstChar == 'L') {
        return "8";
    } else if (firstChar == 'F') {
        return "1";
    } else {
        throw std::logic_error("Обработчик для " + complex + " не установлен");
    }
}

trm::Filters filters{
    {"definition_string", {trm::ArgsFilter::Ignore::SOME, {0}}},
    {"asm", {trm::ArgsFilter::Ignore::ALL}},
    {"error", {trm::ArgsFilter::Ignore::ALL}},
    {"definition", {trm::ArgsFilter::Ignore::NAME_FUNCTION_AND_SLASH}},
    {"call", {trm::ArgsFilter::Ignore::NAME_FUNCTION_AND_SLASH}},
};

}  // namespace

bool Complexoyaz::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Complexoyaz::preprocess(JSON &cmds) {}

void Complexoyaz::postprocess(JSON &cmds)
{
    cmds = processFunctions(cmds);

    for (JSON &cmd : cmds) {
        trm::ArgsRange args{filters, cmd};
        for (auto &arg : args) {
            if (!arg->isString()) {
                continue;
            }
            const std::string argStr = arg->asString();

            //
            // проверка, что комплексы в чистом виде отсутствуют
            //
            std::smatch match;
            static const std::regex isComplex("[LF][0-9]+");
            LCC_ASSERT(!std::regex_match(argStr, match, isComplex));

            //
            // замена L1[i] => comp1_buffer[i]
            //        F1[i] => comp1_buffer[i]
            //
            static const std::regex isComplexWithIndex(R"(([LF])([0-9]+)(\[[^\]]*\]))");
            if (std::regex_match(argStr, match, isComplexWithIndex) && match.size() == 4) {
                std::string prefix = calculateElementSize(argStr) + "byte";
                std::string index = match[3].str();
                std::string complexNumber = match[2].str();
                *arg = prefix + " comp" + complexNumber + "_buffer" + index;
                continue;
            }

            //
            // замена Q1 => 8byte comp1_struct[0]
            //
            static const std::regex isCardinality("Q([0-9]+)");
            if (std::regex_match(argStr, match, isCardinality) && match.size() == 2) {
                *arg = "8byte comp" + match[1].str() + "_struct[0]";
                continue;
            }

            //
            // замена S1 => 8byte comp1_struct[1]
            //
            static const std::regex isCapacity("S([0-9]+)");
            if (std::regex_match(argStr, match, isCapacity) && match.size() == 2) {
                *arg = "8byte comp" + match[1].str() + "_struct[1]";
                continue;
            }
        }
    }
}

trm::Replacers Complexoyaz::makeReplacers()
{
    trm::Replacers replacers;

    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_element_size", {
        const std::string &complexName = tryExtract(patternStringInfo, "complex");
        return calculateElementSize(complexName);
    });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_struct",
                               { return "<complex_id" + patternStringInfo.getGroupAsString() + ">_struct"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_cardinality",
                               { return "8byte <complex_id" + patternStringInfo.getGroupAsString() + ">_struct[0]"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_capacity",
                               { return "8byte <complex_id" + patternStringInfo.getGroupAsString() + ">_struct[1]"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_buffer",
                               { return "8byte <complex_id" + patternStringInfo.getGroupAsString() + ">_struct[2]"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_buffer_opt",
                               { return "<complex_id" + patternStringInfo.getGroupAsString() + ">_buffer"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_cell", {
        const std::string &complexName = tryExtract(patternStringInfo, "complex");
        std::string prefix = calculateElementSize(complexName) + "byte";

        const std::string &group = patternStringInfo.getGroupAsString();

        const trm::Optional<std::string> &param = patternStringInfo.getParam();
        if (!param) {
            throw std::runtime_error("Не указан параметр");
        }

        return prefix + " <complex_id" + group + ">_buffer[" + *param + "]";
    });
    INSERT_FUNCTIONAL_REPLACER(replacers, "string_len", {
        const std::string &content = tryExtract(patternStringInfo, "string");
        return std::to_string(content.size());
    });

    //
    // complex_id комплекса F1 - comp1
    // complex_id комплекса L1 - comp1
    //
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_id", {
        const std::string &complex = tryExtract(patternStringInfo, "complex");
        static const std::regex isComplex("([LF])([0-9]+)");
        std::smatch match;
        if (std::regex_match(complex, match, isComplex) && match.size() == 3) {
            std::string complexNumber = match[2].str();
            return "comp" + complexNumber;
        }
        LCC_FAIL("Matched string is not a complex");
    });

    return replacers;
}

std::string Complexoyaz::getRules()
{
    return std::string(
#include "rules.txt"
    );
}

JSON Complexoyaz::processFunctions(JSON &cmds)
{
    // definition func(a,L1/c,F2)
    // =>
    // definition func(a,comp1_struct/c,comp2_struct)
    // move comp1_buffer, comp1_struct[2]
    // move comp2_buffer, comp2_struct[2]

    // call func(a,L1/c,F2)
    // =>
    // call func(a,comp1_struct/c,comp2_struct)
    // move comp1_buffer, comp1_struct[2]
    // move comp2_buffer, comp2_struct[2]

    JSON result;

    for (JSON &cmd : cmds) {
        LCC_ASSERT(cmd.isMember("type"));
        std::string type = cmd["type"].asString();
        if (type != "cmd") {
            result.append(cmd);
            continue;
        }

        if (!cmd.isMember("args")) {
            result.append(cmd);
            continue;
        }

        std::vector<JSON> postCmds;
        auto processor = [&postCmds](JSON *arg) {
            if (!arg->isString()) {
                return;
            }
            const std::string &argStr = arg->asString();
            static const std::regex isComplex("([LF])([0-9]+)");
            std::smatch match;
            if (std::regex_match(argStr, match, isComplex) && match.size() == 3) {
                JSON args;
                std::string complexNumber = match[2].str();
                args.append("comp" + complexNumber + "_buffer");
                args.append("8byte comp" + complexNumber + "_struct[2]");

                JSON moveCmd;
                moveCmd["type"] = "cmd";
                moveCmd["cmd"] = "move";
                moveCmd["args"] = std::move(args);

                postCmds.push_back(std::move(moveCmd));

                *arg = "comp" + complexNumber + "_struct";
            }
        };

        LCC_ASSERT(cmd.isMember("cmd"));
        std::string cmdName = cmd["cmd"].asString();
        if (cmdName == "definition" || cmdName == "call") {
            trm::ArgsRange args{filters, cmd};
            std::for_each(std::begin(args), std::end(args), processor);
        }

        result.append(cmd);
        for (auto &c : postCmds) {
            result.append(c);
        }
    }

    return result;
}

}  // namespace cyaz
