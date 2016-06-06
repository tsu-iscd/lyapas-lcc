cmake_minimum_required(VERSION 2.8.8)
project(im0_im1)

find_package(BISON)
BISON_TARGET(MyParser im0_im1.y im0_im1.c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ljansson")
set(SOURCE_FILES im0_im1.c print_json.c)
add_executable(im0_im1 ${SOURCE_FILES})
