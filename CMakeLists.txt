cmake_minimum_required(VERSION 2.8.8)
project(main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++1y")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# общие библиотеки
add_subdirectory("sources/libs/backward-cpp")
add_subdirectory("sources/libs/googletest")
add_subdirectory("sources/libs/jsoncpp")

add_subdirectory("sources/libs/print_json")
add_subdirectory("sources/libs/shared_utils")
add_subdirectory("sources/libs/translation_module")

# промежуточные трансляторы
set(COMMON_C_LIBS
    print_json
    "-ljansson")

set(COMMON_CXX_LIBS
    translation_module
    jsoncpp
    shared_utils
    "-ldw")

set(COMMON_TEST_LIBS
    gtest
    "-ldw")

# включаем тесты
enable_testing()

add_subdirectory("sources/lyapas_im0")
add_subdirectory("sources/im0_im1")
add_subdirectory("sources/im1_im2")
add_subdirectory("sources/im2_im3")
add_subdirectory("sources/im4_im5")
