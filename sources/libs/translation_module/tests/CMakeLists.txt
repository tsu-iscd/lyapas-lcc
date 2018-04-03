cmake_minimum_required(VERSION 2.8.8)
project(${TEST_NAME})

add_executable(${PROJECT_NAME}
    ${BACKWARD_ENABLE}
    main.cpp
    example.cpp
    translator_fixture.cpp
    labels_counter.cpp
    variables_counter.cpp
    args_range.cpp
)
target_link_libraries(${PROJECT_NAME}
    ${LIB_NAME}
    ${COMMON_TEST_LIBS})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} "--gtest_output=xml:${CMAKE_BINARY_DIR}/reports/")

