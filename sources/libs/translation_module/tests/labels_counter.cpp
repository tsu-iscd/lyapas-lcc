#include <gtest/gtest.h>
#include <tests_utils/json_parse.h>
#include <translation_module/labels_counter.h>

namespace trm {

TEST(LabelsCounter, usage)
{
    Json::Value program = tests::parse(R"(
[
    {
        "type": "cmd",
        "cmd": "definition",
        "args": ["f1"]
    },
    {
        "type": "label",
        "number": 1
    },
    {
        "type": "label",
        "number": 3
    },
    {
        "type": "cmd",
        "cmd": "definition",
        "args": ["f2"]
    },
    {
        "type": "label",
        "number": 7
    },
    {
        "type": "cmd",
        "cmd": "definition",
        "args": ["f3"]
    }
]
    )");

    LabelsCounter counter;
    counter.process(program);

    auto currentCmd = program.begin();

    //
    // формат комментариев:
    // результат == максимальный номер метки в функции + номер запрашиваемой свободной метки
    //
    // пример:
    // 4 == 3 + 1
    //

    //
    // definition f1
    //
    counter.updateState(*currentCmd);
    // 4 == 3 + 1
    ASSERT_EQ(4, counter.getFree(1));
    // 4 == 3 + 1
    ASSERT_EQ(4, counter.getFree(1));

    // нельзя запросить свободную метку с номером 0
    ASSERT_DEATH(counter.getFree(0), "");

    //
    // label 1
    //
    ++currentCmd;
    counter.updateState(*currentCmd);
    // теперь  максимальный номер метки равен 4,
    // т.к. он был выдан предыдущим вызовом метода 'getFree'
    // 5 == 4 + 1
    ASSERT_EQ(5, counter.getFree(1));
    // 24 == 4 + 20
    ASSERT_EQ(24, counter.getFree(20));

    //
    // label 3
    //
    ++currentCmd;
    counter.updateState(*currentCmd);
    // 30 == 24 + 6
    ASSERT_EQ(30, counter.getFree(6));
    // 25 == 24 + 1
    ASSERT_EQ(25, counter.getFree(1));

    //
    // definition f2
    //
    ++currentCmd;
    counter.updateState(*currentCmd);
    // 10 == 7 + 3
    ASSERT_EQ(10, counter.getFree(3));

    //
    // label 7
    //
    ++currentCmd;
    counter.updateState(*currentCmd);
    ASSERT_EQ(11, counter.getFree(1));

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

}  // namespace trm
