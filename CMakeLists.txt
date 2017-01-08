cmake_minimum_required(VERSION 2.8.8)
project(main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# общие библиотеки
add_subdirectory("sources/libs/print_json")
add_subdirectory("sources/libs/translation_module")

# промежуточные трансляторы
add_subdirectory("sources/lyapas_im0")
add_subdirectory("sources/im0_im1")
add_subdirectory("sources/im1_im2")
add_subdirectory("sources/im2_im3")
add_subdirectory("sources/im4_im5")
