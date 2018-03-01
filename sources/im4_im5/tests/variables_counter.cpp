#include <gtest/gtest.h>
#include <variables_counter.h>
#include <tests_utils/json_parse.h>

namespace cyaz {

TEST(VariablesCounter, usage)
{
    Json::Value program = tests::parse(R"(
[
    {
        "type": "cmd",
        "cmd": "definition",
        "args": ["t10"]
    },
    {
        "type": "cmd",
        "cmd": "super_cmd",
        "args": ["t1", "t3"]
    },
    {
        "type": "cmd",
        "cmd": "write_string",
        "args": ["t10", "t30"]
    },
    {
        "type": "cmd",
        "cmd": "asm",
        "args": ["t10", "t30"]
    },
    {
        "type": "cmd",
        "cmd": "insert_string_in_complex",
        "args": ["t10", "t30"]
    },
    {
        "type": "cmd",
        "cmd": "definition",
        "args": ["f2"]
    },
    {
        "type": "cmd",
        "cmd": "call",
        "args": ["t10", "t5"]
    },
    {
        "type": "cmd",
        "cmd": "definition",
        "args": ["f3"]
    }
]
    )");

    VariablesCounter counter;
    counter.process(program);

    auto currentCmd = program.begin();

    //
    // формат комментариев:
    // результат == максимальный номер переменной типа "tN" в функции +
    //              номер запрашиваемой свободной переменной
    //
    // пример:
    // 4 == 3 + 1
    //

    //
    // definition t10
    // t10 -- имя функции. Важно, что оно не считается за пременную
    //
    counter.updateState(*currentCmd);
    // 4 == 3 + 1
    ASSERT_EQ(4, counter.getFree(1));
    // 4 == 3 + 1
    ASSERT_EQ(4, counter.getFree(1));

    // нельзя запросить свободную переменную с номером 0
    ASSERT_DEATH(counter.getFree(0), "");

    //
    // super_cmd t1, t3
    //
    ++currentCmd;
    counter.updateState(*currentCmd);
    // после транслции следующей команды переменная t4 снова свободна
    // 4 == 3 + 1
    ASSERT_EQ(4, counter.getFree(1));
    // 23 == 3 + 20
    ASSERT_EQ(23, counter.getFree(20));

    // важно, что строки t10 и t20 в следующих командах
    // не считаются за переменные

    // write_string t10, t30
    ++currentCmd;
    counter.updateState(*currentCmd);
    // asm t10, t30
    ++currentCmd;
    counter.updateState(*currentCmd);
    // insert_string_in_complex t10, t30
    ++currentCmd;
    counter.updateState(*currentCmd);

    // 4 == 3 + 1
    ASSERT_EQ(4, counter.getFree(1));


    //
    // definition f2
    //
    ++currentCmd;
    counter.updateState(*currentCmd);
    // 8 == 5 + 3
    ASSERT_EQ(8, counter.getFree(3));

    //
    // call t10, t5
    //
    ++currentCmd;
    counter.updateState(*currentCmd);
    // 6 == 5 + 1
    ASSERT_EQ(6, counter.getFree(1));

    //
    // definition f3
    //
    ++currentCmd;
    counter.updateState(*currentCmd);
    // 1 == 0 + 1
    ASSERT_EQ(1, counter.getFree(1));

    // программа закончилась
    ASSERT_EQ(program.end(), ++currentCmd);
}

}
