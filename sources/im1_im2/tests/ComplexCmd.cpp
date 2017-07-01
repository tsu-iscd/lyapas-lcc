#include "CompositeCmd/Composites/ComplexCmd.h"
#include <gtest/gtest.h>
#include "TreeParser/TreeParser.h"

//TODO(vsafonov): тесты можно сделать проще, если перейти на json либу nlohmann

class ComplexCmdFixture : public ::testing::Test {
protected:
    CompositeCmd::SPtrComposite createCmd(const Json::Value &json)
    {
        Json::Value wrappedJson;
        wrappedJson.append(json);
        return parser.parseTree(wrappedJson);
    }

    CmdFactory factory;
    TreeParser parser{factory};
};

TEST_F(ComplexCmdFixture, symbolComplex)
{
    Json::Value complexJson;
    complexJson["type"] = "symbol_complex";
    complexJson["number"] = 1;
    auto complex = createCmd(complexJson);


    Json::Value cmd = complex->toJson();
    ASSERT_EQ(1, cmd.size());
    ASSERT_TRUE(cmd.isMember(fieldName::args));

    Json::Value &args = cmd[fieldName::args];
    ASSERT_EQ(1, args.size());
    ASSERT_EQ("F1", args[0].asString());
}

TEST_F(ComplexCmdFixture, logicComplex)
{
    Json::Value complexJson;
    complexJson["type"] = "logic_complex";
    complexJson["number"] = 10;
    auto complex = createCmd(complexJson);


    Json::Value cmd = complex->toJson();
    ASSERT_EQ(1, cmd.size());
    ASSERT_TRUE(cmd.isMember(fieldName::args));

    Json::Value &args = cmd[fieldName::args];
    ASSERT_EQ(1, args.size());
    ASSERT_EQ("L10", args[0].asString());
}

TEST_F(ComplexCmdFixture, globalComplex1)
{
    Json::Value complexJson;
    complexJson["type"] = "global_complex_1";
    auto complex = createCmd(complexJson);


    Json::Value cmd = complex->toJson();
    ASSERT_EQ(1, cmd.size());
    ASSERT_TRUE(cmd.isMember(fieldName::args));

    Json::Value &args = cmd[fieldName::args];
    ASSERT_EQ(1, args.size());
    ASSERT_EQ("G1", args[0].asString());
}

TEST_F(ComplexCmdFixture, globalComplex4)
{
    Json::Value complexJson;
    complexJson["type"] = "global_complex_4";
    auto complex = createCmd(complexJson);


    Json::Value cmd = complex->toJson();
    ASSERT_EQ(1, cmd.size());
    ASSERT_TRUE(cmd.isMember(fieldName::args));

    Json::Value &args = cmd[fieldName::args];
    ASSERT_EQ(1, args.size());
    ASSERT_EQ("G4", args[0].asString());
}
