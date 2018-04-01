#include "complexoyaz.h"
#include <memory>
#include <stdexcept>
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

}  // namespace

bool Complexoyaz::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Complexoyaz::preprocess(JSON &cmds) {}

void Complexoyaz::postprocess(JSON &cmds) {}

trm::Replacers Complexoyaz::makeReplacers()
{
    trm::Replacers replacers;

    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_element_size", {
        const std::string &complexName = tryExtract(patternStringInfo, "complex");
        return calculateElementSize(complexName);
    });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_struct",
                               { return "<complex" + patternStringInfo.getGroupAsString() + ">_struct"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_cardinality",
                               { return "8byte <complex" + patternStringInfo.getGroupAsString() + ">_struct[0]"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_capacity",
                               { return "8byte <complex" + patternStringInfo.getGroupAsString() + ">_struct[1]"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_buffer",
                               { return "8byte <complex" + patternStringInfo.getGroupAsString() + ">_struct[2]"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_buffer_opt",
                               { return "<complex" + patternStringInfo.getGroupAsString() + ">_buffer"; });
    INSERT_FUNCTIONAL_REPLACER(replacers, "complex_cell", {
        const std::string &complexName = tryExtract(patternStringInfo, "complex");
        std::string prefix = calculateElementSize(complexName) + "byte";

        const std::string &group = patternStringInfo.getGroupAsString();

        const trm::Optional<std::string> &param = patternStringInfo.getParam();
        if (!param) {
            throw std::runtime_error("Не указан параметр");
        }

        return prefix + " <complex" + group + ">_buffer[" + *param + "]";
    });
    INSERT_FUNCTIONAL_REPLACER(replacers, "string_len", {
        const std::string &content = tryExtract(patternStringInfo, "string");
        return std::to_string(content.size());
    });

    return replacers;
}

std::string Complexoyaz::getRules()
{
    return std::string(
#include "rules.txt"
    );
}

}  // namespace cyaz
