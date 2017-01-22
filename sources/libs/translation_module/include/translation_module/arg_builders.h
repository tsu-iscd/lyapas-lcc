#pragma once

#include <string>
#include <memory>
#include "aliases.h"
#include "replacers.h"
#include "cmd_translator_storage.h"

namespace trm {

class Packer {
public:
    virtual ~Packer() = default;
    virtual Json::Value pack(const std::string &value) = 0;
};
using PackerPtr = std::shared_ptr<Packer>;

class IntPacker : public Packer {
public:
    Json::Value pack(const std::string &value) override
    {
        return Json::Value(std::stoll(value));
    }
};

class StringPacker : public Packer {
public:
    Json::Value pack(const std::string &value) override
    {
        return Json::Value(value);
    }
};

class ArgBuilder {
public:
    virtual ~ArgBuilder() = default;
    virtual Json::Value createArg() = 0;
};

class ConstArgBuilder : public ArgBuilder {
public:
    ConstArgBuilder(const Json::Value &sample) :
        sample(sample)
    {
    }

    Json::Value createArg() override
    {
        return sample;
    }

private:
    Json::Value sample;
};

class JsonArgBuilder : public ArgBuilder {
public:
    JsonArgBuilder(const std::string &fieldName, JsonMap &jsonMap) :
        fieldName(fieldName),
        jsonMap(jsonMap)
    {
    }

    Json::Value createArg() override
    {
        return jsonMap[fieldName];
    }

private:
    std::string fieldName;
    JsonMap &jsonMap;
};

class PatternArgBuilder : public ArgBuilder {
public:
    PatternArgBuilder(PackerPtr packer, const std::string patternedString, const CmdTranslatorStorage &storage) :
        packer(packer),
        patternedString(patternedString),
        storage(storage)
    {
    }

    Json::Value createArg() override
    {
        auto findPair = [](const StringView &str, char lchar, char rchar) -> StringView {
            Optional<unsigned> lindex;
            int count = 0;

            for (unsigned i = 0; i < str.size(); ++i) {
                char c = str[i];
                if (c == lchar) {
                    if (count == 0) {
                        lindex = i;
                    }
                    ++count;
                } else if (c == rchar && bool(lindex)) {
                    --count;
                    if (count == 0) {
                        return {str.data() + *lindex, i - *lindex + 1};
                    }
                }
            }

            return {str.data(), 0};
        };

        auto findMostDeepPair = [&](const StringView &str, char lchar, char rchar) -> std::pair<bool, StringView> {
            auto foundPair = findPair(str, lchar, rchar);
            if (foundPair.empty()) {
                return {false, {nullptr, 0}};
            }

            while (true) {
                auto newFoundPair = findPair(foundPair.substr(1, foundPair.size() - 2), lchar, rchar);
                if (newFoundPair.empty()) {
                    return {true, foundPair};
                }
                foundPair = newFoundPair;
            }
        };

        std::string workString(patternedString);
        while(true) {
            StringView sw(workString.data(), workString.size());
            auto result = findMostDeepPair(sw, '<', '>');

            if (result.first) {
                StringView &foundSubstr = result.second;
                auto replaceFrom = std::distance(workString.data(), foundSubstr.data());
                auto replaceSize = foundSubstr.size();

                PatternStringInfo patternStringInfo(std::string(foundSubstr.data() + 1,
                                                                foundSubstr.size() - 2),
                                                    storage.srcArgString);

                auto p = storage.srcArgString.find(patternStringInfo.getNameWithGroup());
                if (p != storage.srcArgString.end()) {
                    const std::string &newValue = p->second;

                    workString.replace(replaceFrom, replaceSize, newValue);
                    continue;
                }

                auto replacer = storage.replacers.find(patternStringInfo.getName());
                if (replacer != storage.replacers.end()) {
                    const std::string &newValue = replacer->second->resolve(patternStringInfo);

                    workString.replace(replaceFrom, replaceSize, newValue);
                    continue;
                }

                throw std::runtime_error("Нет замены для шаблона " + patternStringInfo.getName());
            } else {
                return packer->pack(std::string(sw.data(), sw.size()));
            }
        }
    }

    PackerPtr packer;
    const std::string patternedString;
    const CmdTranslatorStorage &storage;
};

std::shared_ptr<ArgBuilder> createArgBuilder(std::string value, CmdTranslatorStorage &storage)
{
    //FIXME(vsafonov): DRY, эти функции повторяеются в fillers.h
    auto eraseBorders = [](std::string &str) {
        if (str.size() >= 2) {
            str = std::string(str.begin() + 1, str.end() - 1);
        } else {
            throw std::runtime_error("string is too small");
        }
    };

    auto isBorderedBy = [](const std::string &str, char b, char e) {
        if (str.size() < 2) {
            return false;
        }
        return str.front() == b && str.back() == e;
    };

    // {} => JsonArgBuilder
    if (isBorderedBy(value, '{', '}')) {
        eraseBorders(value);
        return std::make_shared<JsonArgBuilder>(value, storage.srcArgJson);
    }

    // int, "string" => ConstArgBuilder
    // <> <>, "<> <>" => PatternArgBuilder
    PackerPtr packer;
    if (isBorderedBy(value, '"', '"')) {
        eraseBorders(value);
        packer = std::make_shared<StringPacker>();
    } else {
        packer = std::make_shared<IntPacker>();
    }

    auto left = value.find('<');
    auto right = value.rfind('>');
    if (left != std::string::npos && right != std::string::npos && left < right) {
        return std::make_shared<PatternArgBuilder>(packer, value, storage);
    } else {
        return std::make_shared<ConstArgBuilder>(packer->pack(value));
    }
}

}
