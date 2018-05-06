#include <complexoyaz.h>
#include <gtest/gtest.h>
#include <tests_utils/json_parse.h>

namespace cyaz {

class ComplexoyazFixture : public ::testing::Test {
public:
    void postprocess(JSON &cmds)
    {
        tr.postprocess(cmds);
    }

private:
    Complexoyaz tr;
};

TEST_F(ComplexoyazFixture, postprocess)
{
    Json::Value program = tests::parse(R"(
[
    {
        "type": "cmd",
        "cmd": "definition",
        "args": ["func1", "L1", "/", "L2"]
    },
    {
        "type": "cmd",
        "cmd": "move",
        "args": ["t3", "L1[t2]"]
    },
    {
        "type": "cmd",
        "cmd": "move",
        "args": ["L2[0]", "t3"]
    },
    {
        "type": "cmd",
        "cmd": "move",
        "args": ["L2[1]", "S1"]
    },
    {
        "type": "cmd",
        "cmd": "move",
        "args": ["L2[2]", "Q1"]
    }
]
    )");

    postprocess(program);

    Json::Value expected = tests::parse(R"(
[
    {
        "type": "cmd",
        "cmd": "definition",
        "args" : ["func1", "L1_struct", "/", "L2_struct"]
    },
    {
        "type" : "cmd",
        "cmd" : "move",
        "args" : ["L1_buffer", "8byte L1_struct[2]"]
    },
    {
        "type" : "cmd",
        "cmd" : "move",
        "args" : ["L2_buffer", "8byte L2_struct[2]"]
    },
    {
        "type" : "cmd",
        "cmd" : "move",
        "args" : ["t3", "8byte L1_buffer[t2]"]
    },
    {
        "type" : "cmd",
        "cmd" : "move",
        "args" : ["8byte L2_buffer[0]", "t3"]
    },
    {
        "type" : "cmd",
        "cmd" : "move",
        "args" : ["8byte L2_buffer[1]", "8byte L1_struct[1]"]
    },
    {
        "type" : "cmd",
        "cmd" : "move",
        "args" : ["8byte L2_buffer[2]", "8byte L1_struct[0]"]
    }
]
    )");
    ASSERT_EQ(expected, program);
}

TEST_F(ComplexoyazFixture, postprocessIgnoreStrings)
{
    Json::Value program = tests::parse(R"(
[
    {
        "type" : "cmd",
        "cmd" : "asm",
        "args" : ["L1[t1]"]
    },
    {
        "type": "cmd",
        "cmd": "definition_string",
        "args": ["L1[t1]", "t1"]
    },
    {
        "type": "cmd",
        "cmd": "error",
        "args": ["L1[t1]"]
    },
    {
        "type": "cmd",
        "cmd": "definition",
        "args": ["L1", "/"]
    },
    {
        "type": "cmd",
        "cmd": "call",
        "args": ["t1", "/"]
    }
]
    )");

    Json::Value expected = program;
    postprocess(program);
    ASSERT_EQ(expected, program);
}

}  // namespace cyaz

/*
 */
