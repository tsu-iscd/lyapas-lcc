#include <gtest/gtest.h>
#include <tests_utils/json_parse.h>
#include "CompositeCmd/Composites/ProcedureCmd.h"
#include "TreeParser/TreeParser.h"

class ProcedureCmdFixture : public ::testing::Test {
protected:
    CompositeCmd::SPtrComposite createCmd(const std::string &type,
                                          const std::string &name,
                                          const std::string &inArg,
                                          const std::string &outArg)
    {
        Json::Value procedure;
        procedure.append(makeHeader(type, name));
        procedure.append(makeArgs("in_args", inArg));
        procedure.append(makeArgs("out_args", outArg));

        return parser.parseTree(procedure);
    }

private:
    Json::Value makeHeader(const std::string &type, const std::string &name)
    {
        Json::Value header;
        header["type"] = type;
        header["name"] = name;
        return header;
    }

    Json::Value makeArgs(const std::string &argsType, const std::string &name)
    {
        Json::Value header;
        header["type"] = argsType;

        Json::Value arg;
        arg["type"] = "var";
        arg["name"] = name;

        Json::Value args;
        args.append(arg);

        Json::Value result;
        result.append(header);
        result.append(args);
        return result;
    }

    CmdFactory factory;
    TreeParser parser{factory};
};

TEST_F(ProcedureCmdFixture, definition)
{
    auto procedure = createCmd("procedure", "f1", "a", "b");

    Json::Value expected = tests::parse(R"(
[
    {
        "type" : "cmd",
        "cmd" : "definition",
        "args" : ["f1", "a", "/", "b"]
    }
]
    )");
    const Json::Value &actual = procedure->toJson();
    ASSERT_EQ(expected, actual);
}

TEST_F(ProcedureCmdFixture, call)
{
    auto procedure = createCmd("call", "f1", "a", "b");

    Json::Value expected = tests::parse(R"(
{
    "type" : "cmd",
    "cmd" : "call",
    "args" : ["f1", "a", "/", "b"]
}
    )");
    const Json::Value &actual = procedure->toJson();
    ASSERT_EQ(expected, actual);
}
