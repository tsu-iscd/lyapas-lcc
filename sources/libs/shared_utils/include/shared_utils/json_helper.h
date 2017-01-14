#pragma once

#include <iostream>
#include <fstream>
#include <json/json.h>

namespace sutils {

void parseJsonFromStream(std::istream &stream, Json::Value &json);
void parseJsonFromFile(const char *name, Json::Value &json);
void writeJsonToStream(std::ostream &stream, const Json::Value &json);

}