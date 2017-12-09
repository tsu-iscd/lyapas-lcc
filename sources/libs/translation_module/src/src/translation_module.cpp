#include "translation_module.h"
#include <string>
#include <tuple>
#include <utility>
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

using CmdTranslators = std::map<CmdInfo, trm::CmdTranslator>;

// парсинг: type[/name] [arg1, arg2, arg3 ...]
CmdInfo parseCmdInfoFromString(const std::string line)
{
    // lambda для парсинга type[/name]
    auto parseNameAndType = [](std::string &type, std::string &name) {
        auto slash = type.find('/');
        if (slash != std::string::npos) {
            name = std::string(type.begin() + slash + 1, type.end());
            type = type.substr(0, slash);
        }
    };

    // type args
    auto firstSpace = line.find(' ');
    if (firstSpace == std::string::npos) {
        std::string type = line;
        std::string name;
        parseNameAndType(type, name);
        return {type, name, {}};
    }
    std::string type(line.begin(), line.begin() + firstSpace);
    std::istringstream argsStream(std::string(line.begin() + firstSpace + 1, line.end()));

    // type[/name]
    std::string name;
    parseNameAndType(type, name);

    // arg1, arg2, arg3 ...
    std::vector<std::string> args;
    std::string arg;
    while (std::getline(argsStream, arg, ',')) {
        sutils::lrstrip(arg);
        args.push_back(arg);
    }

    return {type, name, std::move(args)};
}

CmdTranslators getCmdTranslators(const std::string &rules, const Replacers &replacers)
{
    CmdTranslators cmdTranslators;

    std::istringstream rulesStream(rules);
    std::string line;
    size_t lineNo = 0;
    while (std::getline(rulesStream, line)) {
        ++lineNo;

        // пропускаем пустые строки и комментариии
        sutils::lrstrip(line);
        if (line.empty() || line.front() == '#') {
            continue;
        }

        // считываем транслируемую команду
        auto srcCmd = parseCmdInfoFromString(line);

        // пропускаем =>
        std::getline(rulesStream, line);
        sutils::lrstrip(line);
        if (line != "=>") {
            throw std::runtime_error("Формат файла с правилами трансляции нарушен, строка: " +
                                     std::to_string(lineNo));
        }

        // считываем набор команд
        std::vector<CmdInfo> dstCmds;
        while(std::getline(rulesStream, line)) {
            sutils::lrstrip(line);
            if (line.empty() || line.front() == '#') {
                break;
            }

            dstCmds.push_back(parseCmdInfoFromString(line));
        }

        auto p = cmdTranslators.emplace(std::piecewise_construct,
                                        std::forward_as_tuple(srcCmd),
                                        std::forward_as_tuple(srcCmd, dstCmds, replacers));
        if (!p.second) {
            throw std::runtime_error("Команда для трансляции повторяется более одного раза, строка: " +
                                     std::to_string(lineNo));
        }
    }

    return cmdTranslators;
}

void trm::TranslationModule::process(JSON &cmds)
{
    Replacers &replacers = getReplacers(cmds);
    static CmdTranslators cmdTranslators = getCmdTranslators(getRules(), replacers);

    if (!cmds.isArray()) {
        throw std::runtime_error("JSON with commands is not array");
    }

    // TODO(vsafonov): оптимизировать этот цикл.
    // 1) работать в массиве cmds, а не resultCmds.
    // 2) пропускать команды, которые не требуют трансляции.
    JSON resultCmds;
    for (auto &&cmd : cmds) {
        for (auto &replacer : replacers) {
            replacer.second->updateState(cmd);
        }

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
