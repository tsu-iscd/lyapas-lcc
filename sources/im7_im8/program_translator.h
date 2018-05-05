#pragma once

#include "program.h"
#include <map>
#include <string>

namespace ayaz {

class ProgramTranslator {
public:
    using StringStorage = std::map<std::string, std::string>;

    void process(Program &program);
    const StringStorage &getStringStorage();

private:
    StringStorage strings;
};

}
