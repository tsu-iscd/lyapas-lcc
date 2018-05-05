#pragma once

#include <string>
#include <translation_module/aliases.h>

namespace ayaz {

JSON makeCmd(const std::string &name, const std::initializer_list<JSON> &args = {});
JSON makeLabel(const std::string &name);

}
