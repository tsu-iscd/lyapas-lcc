#pragma once

#include <list>
#include <translation_module/aliases.h>

namespace ayaz {

using Program = std::list<JSON>;

Program transform(const JSON &cmds);
JSON transform(const Program &program);

}

