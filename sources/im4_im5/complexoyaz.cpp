#include "complexoyaz.h"
#include "replacers.h"
#include <memory>

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
        throw std::logic_error("Обработчик не установлен");
    }
}

}  // namespace

bool Complexoyaz::valid(const JSON &cmds, std::string &error)
{
    return true;
}

void Complexoyaz::preprocess(JSON &cmds) {}

void Complexoyaz::postprocess(JSON &cmds) {}

#define INSERT_REPLACER(name, type) replacers.insert({name, std::make_shared<type>(cmds)})

#define INSERT_FUNCTIONAL_REPLACER(name, functionBlock)                                                    \
    do {                                                                                                   \
        auto func = [&](const trm::PatternStringInfo &patternStringInfo) -> std::string { functionBlock }; \
        replacers.insert({name, std::make_shared<cyaz::FunctionalReplacer>(func)});                        \
    } while (false)

trm::Replacers &Complexoyaz::getReplacers(const JSON &cmds)
{
    static trm::Replacers replacers;

    INSERT_REPLACER("free_var", cyaz::FreeVarReplacer);
    INSERT_REPLACER("free_label", cyaz::FreeLabelReplacer);

    INSERT_FUNCTIONAL_REPLACER("complex_element_size", {
        const std::string &complexName = tryExtract(patternStringInfo, "complex");
        return calculateElementSize(complexName);
    });
    INSERT_FUNCTIONAL_REPLACER("complex_struct",
                               { return "<complex" + patternStringInfo.getGroupAsString() + ">_struct"; });
    INSERT_FUNCTIONAL_REPLACER("complex_cardinality",
                               { return "8byte <complex" + patternStringInfo.getGroupAsString() + ">_struct[0]"; });
    INSERT_FUNCTIONAL_REPLACER("complex_capacity",
                               { return "8byte <complex" + patternStringInfo.getGroupAsString() + ">_struct[1]"; });
    INSERT_FUNCTIONAL_REPLACER("complex_buffer",
                               { return "8byte <complex" + patternStringInfo.getGroupAsString() + ">_struct[2]"; });
    INSERT_FUNCTIONAL_REPLACER("complex_buffer_opt",
                               { return "<complex" + patternStringInfo.getGroupAsString() + ">_buffer"; });
    INSERT_FUNCTIONAL_REPLACER("complex_cell", {
        const std::string &complexName = tryExtract(patternStringInfo, "complex");
        std::string byteSize = calculateElementSize(complexName) + "byte";

        const std::string &group = patternStringInfo.getGroupAsString();

        const trm::Optional<std::string> &param = patternStringInfo.getParam();
        if (!param) {
            throw std::runtime_error("Не указан параметр");
        }

        return byteSize + " <complex" + group + ">_buffer[" + *param + "]";
    });
    INSERT_FUNCTIONAL_REPLACER("string_len", {
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
