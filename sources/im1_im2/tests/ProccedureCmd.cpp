#include <gtest/gtest.h>
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

    Json::Value cmd = procedure->toJson();
    ASSERT_TRUE(cmd.isArray());
    ASSERT_EQ(1, cmd.size());

    const Json::Value &content = cmd[0];
    ASSERT_TRUE(content.isMember(fieldName::args));
    ASSERT_TRUE(content.isMember(fieldName::type));

    const Json::Value &args = content["args"];
    ASSERT_EQ(4, args.size());
    ASSERT_EQ("f1", args[0].asString());
    ASSERT_EQ("a", args[1].asString());
    ASSERT_EQ("/", args[2].asString());
    ASSERT_EQ("b", args[3].asString());

    const Json::Value &type = content["type"];
    ASSERT_EQ("definition", type.asString());
}

TEST_F(ProcedureCmdFixture, call)
{
    auto procedure = createCmd("call", "f1", "a", "b");

    const Json::Value &content = procedure->toJson();
    ASSERT_TRUE(content.isMember(fieldName::args));
    ASSERT_TRUE(content.isMember(fieldName::type));

    const Json::Value &args = content["args"];
    ASSERT_EQ(4, args.size());
    ASSERT_EQ("f1", args[0].asString());
    ASSERT_EQ("a", args[1].asString());
    ASSERT_EQ("/", args[2].asString());
    ASSERT_EQ("b", args[3].asString());

    const Json::Value &type = content["type"];
    ASSERT_EQ("call", type.asString());
}
