#include "translation_module.h"
#include <string>
#include <vector>
#include <shared_utils/string_helper.h>
#include "cmd_translator.h"
#include "cmd_info.h"

namespace trm {

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

std::map<CmdInfo, trm::CmdTranslator> getCmdTranslators(const std::string &rules)
{
    std::map<CmdInfo, trm::CmdTranslator> cmdTranslators;

    std::istringstream rulesStream(rules);
    std::string line;
    while (std::getline(rulesStream, line)) {
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
        std::istringstream argsStream(std::string(line.begin() + colon + 1, line.end()));

        // type[/name]
        std::string name;
        auto slash = type.find('/');
        if (slash != std::string::npos) {
            name = std::string(type.begin() + slash + 1, type.end());
            type = type.substr(0, slash);
        }

        std::vector<std::string> args;
        // arg1, arg2, arg3 ...
        std::string arg;
        while (std::getline(argsStream, arg, ',')) {
            sutils::lrstrip(arg);
            args.push_back(arg);
        }

        // TODO(vsafonov):
        // 1) пропустить =>
        // 2) парсить команды до пустой строки

        cmdTranslators.emplace(CmdInfo{type, name}, args);
    }

    return cmdTranslators;
}

void trm::TranslationModule::process(JSON &cmds)
{
    static std::map<CmdInfo, trm::CmdTranslator> cmdTranslators = getCmdTranslators(getRules());

    if (!cmds.isArray()) {
        throw std::runtime_error("JSON with commands is not array");
    }

    // TODO(vsafonov): оптимизировать этот цикл.
    // 1) работать в массиве cmds, а не resultCmds.
    // 2) пропускать команды, которые не требуют трансляции.
    JSON resultCmds;
    for (auto &&cmd : cmds) {
        auto p = cmdTranslators.find(createCmdInfo(cmd));
        if (p != cmdTranslators.end()) {
            for (auto &&resultCmd : p->second.translate(cmd)) {
                resultCmds.append(std::move(resultCmd));
            }
        } else {
            resultCmds.append(std::move(cmd));
        }
    }

    cmds = std::move(resultCmds);
}

}
