cmake_minimum_required(VERSION 2.8.8)
project(im0_im1)

find_package(BISON)
BISON_TARGET(MyParser im0_im1.y ${CMAKE_BINARY_DIR}/im0_im1.c)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#
include_directories(${CMAKE_SOURCE_DIR})
set(SOURCE_FILES im0_im1.c print_json.c)
add_executable(im0_im1 ${SOURCE_FILES})

target_link_libraries(im0_im1 "-ljansson")
