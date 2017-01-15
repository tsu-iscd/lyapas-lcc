#include "translation_module.h"

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

void trm::TranslationModule::process(JSON &cmds)
{
    std::string designations = getDesignations();
    std::string rules = getRules();
    // трансляция по конфиг-файлу
}
