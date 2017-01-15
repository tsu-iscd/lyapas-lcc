#pragma once

#include <stdexcept>
#include <string>
#include <json/json.h>

typedef Json::Value JSON;

namespace trm {

class TranslationModule {
public:
    void translate(JSON &cmds);

private:
    virtual bool valid(const JSON &cmds, std::string &error);
    virtual void preprocess(JSON &cmds) = 0;
    // поведение этой функции опеределяется конфиг файлами трансляции
    virtual void process(JSON &cmds) final;
    virtual void postprocess(JSON &cmds) = 0;

    virtual std::string getDesignations() = 0;
    virtual std::string getRules() = 0;
};

}
