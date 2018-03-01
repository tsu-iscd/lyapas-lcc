cmake_minimum_required(VERSION 2.8.8)
project(${TEST_NAME})

set(SOURCE_FILES
    main.cpp
    labels_counter.cpp
    variables_counter.cpp
)

add_executable(${TEST_NAME} ${SOURCE_FILES} ${BACKWARD_ENABLE})
target_link_libraries(${TEST_NAME} ${LIB_NAME} ${COMMON_TEST_LIBS})

add_test(
    NAME ${TEST_NAME}
    COMMAND ${PROJECT_NAME}
    "--gtest_output=xml:${CMAKE_BINARY_DIR}/reports/"
)
