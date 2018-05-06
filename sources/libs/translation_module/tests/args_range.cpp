#include <gtest/gtest.h>
#include <tests_utils/json_parse.h>
#include <translation_module/args_range.h>

namespace trm {

TEST(ArgsRange, usage)
{
    Filters filters{{"concat", {ArgsFilter::Ignore::ALL}},
                    {"definition", {ArgsFilter::Ignore::SOME, {0}}},
                    {"test_cmd", {ArgsFilter::Ignore::SOME, {0, 2}}},
                    {"call", {ArgsFilter::Ignore::NAME_FUNCTION_AND_SLASH}}};

    //
    // в команде фильтруется название и /
    //
    {
        Json::Value cmd = tests::parse(R"(
            {
            
                "type": "cmd",
                "cmd": "call",
                "args": ["func", "a", "/", "b"]
             }
        )");
        ArgsRange range{filters, cmd};
        std::vector<JSON *> args{range.begin(), range.end()};

        ASSERT_EQ(2, args.size());
        ASSERT_EQ("a", args[0]->asString());
        ASSERT_EQ("b", args[1]->asString());
    }

    //
    // в команде фильтруется первый аргумент
    //
    {
        Json::Value cmd = tests::parse(R"(
            {
                "type": "cmd",
                "cmd": "definition",
                "args": ["func", "a", "/", "b"]
            }
        )");
        ArgsRange range{filters, cmd};
        std::vector<JSON *> args{range.begin(), range.end()};
        ASSERT_EQ(3, args.size());
        ASSERT_EQ("a", args[0]->asString());
        ASSERT_EQ("/", args[1]->asString());
        ASSERT_EQ("b", args[2]->asString());
    }

    //
    // в команде фильтруются все аргументы
    //
    {
        Json::Value cmd = tests::parse(R"(
            {
                "type": "cmd",
                "cmd": "concat",
                "args": ["Hello, ", "world!"]
            }
        )");
        ArgsRange range{filters, cmd};
        std::vector<JSON *> args{range.begin(), range.end()};
        ASSERT_TRUE(args.empty());
    }

    //
    // JSON не является командой
    //
    {
        Json::Value cmd = tests::parse(R"(
            {
                "type": "label",
                "number": 1
            }
        )");
        ArgsRange range{filters, cmd};
        std::vector<JSON *> args{range.begin(), range.end()};
        ASSERT_TRUE(args.empty());
    }

    //
    // в команде фильтруются первый и третий аргументы
    //
    {
        Json::Value cmd = tests::parse(R"(
            {
                "type": "cmd",
                "cmd": "test_cmd",
                "args": ["a", "b", "c"]
            }
        )");
        ArgsRange range{filters, cmd};
        std::vector<JSON *> args{range.begin(), range.end()};
        ASSERT_EQ(1, args.size());
        ASSERT_EQ("b", args[0]->asString());
    }

    //
    // команда отсутствует в фильтрах
    //
    {
        Json::Value cmd = tests::parse(R"(
            {
                "type": "cmd",
                "cmd": "another_cmd",
                "args": ["a", "b"]
            }
        )");
        ArgsRange range{filters, cmd};
        std::vector<JSON *> args{range.begin(), range.end()};
        ASSERT_EQ(2, args.size());
        ASSERT_EQ("a", args[0]->asString());
        ASSERT_EQ("b", args[1]->asString());
    }

    //
    // в команде фильтруются первый и третий аргументы,
    // но аргументы отсутствуют
    //
    {
        Json::Value cmd = tests::parse(R"(
            {
                "type": "cmd",
                "cmd": "test_cmd",
                "args": []
            }
        )");
        ArgsRange range{filters, cmd};
        std::vector<JSON *> args{range.begin(), range.end()};
        ASSERT_TRUE(args.empty());
    }
}

}  // namespace trm
