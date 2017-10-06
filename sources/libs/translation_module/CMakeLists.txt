cmake_minimum_required(VERSION 2.8.8)
project(translation_module)

set(LIB_NAME "${PROJECT_NAME}")
set(TEST_NAME "${PROJECT_NAME}_test")

add_subdirectory(src)
add_subdirectory(tests)
