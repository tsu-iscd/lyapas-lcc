#include "translation_module.h"
#include <vector>
#include <shared_utils/string_helper.h>
#include "cmd_translator.h"

trm::TranslationModule::~TranslationModule()
{
}

void trm::TranslationModule::translate(JSON &cmds)
{
    std::string error;
    if (valid(cmds, error)) {
        preprocess(cmds);
        process(cmds);
        postprocess(cmds);
    } else {
        throw std::runtime_error("commands is not valid: " + error);
    }
}

bool trm::TranslationModule::valid(const JSON &cmds, std::string &error)
{
    return true;
}

trm::CmdTranslator trm::TranslationModule::getCmdTranslator()
{
    std::vector<std::vector<ArgBuilder>> cmdsArgBulders;

    std::istringstream rules(getRules());
    std::string line;
    while (std::getline(rules, line)) {
        // парсинг: type[/name]: arg1, arg2, arg3 ...

        // пропускаем пустые строки и комментариии
        sutils::lrstrip(line);
        if (line.empty() || line.front() == '#') {
            continue;
        }

        // type: args
        auto colon = line.find(':');
        if (colon == std::string::npos) {
            throw std::runtime_error("Invalid rules file");
        }
        std::string type(line.begin(), line.begin() + colon);
        std::istringstream args(std::string(line.begin() + colon + 1, line.end()));

        // type[/name]
        std::string name;
        auto slash = type.find('/');
        if (slash != std::string::npos) {
            name = std::string(type.begin() + slash + 1, type.end());
            type = type.substr(0, slash);
        }

        std::vector<ArgBuilder> argBuilders;
        // arg1, arg2, arg3 ...
        std::string arg;
        while (std::getline(args, arg, ',')) {
            sutils::lrstrip(arg);
            argBuilders.push_back(trm::createArgBuilder(arg));
        }
        cmdsArgBulders.push_back(std::move(argBuilders));
    }

    // TODO(vsafonov): добавить коллекцию с заполнителями
    return {std::move(cmdsArgBulders), {}};
}

void trm::TranslationModule::process(JSON &cmds)
{
    static trm::CmdTranslator cmdTranslator = getCmdTranslator();

    if (!cmds.isArray()) {
        throw std::runtime_error("JSON with commands is not array");
    }

    // TODO(vsafonov): оптимизировать этот цикл.
    // 1) работать в массиве cmds, а не resultCmds.
    // 2) пропускать команды, которые не требуют трансляции.
    JSON resultCmds;
    for (auto &&cmd : cmds) {
        std::vector<JSON> newCmds = cmdTranslator.translate(cmd);
        for (JSON &newCmd : newCmds) {
            resultCmds.append(std::move(newCmd));
        }
    }

    cmds = std::move(resultCmds);
}
