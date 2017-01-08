cmake_minimum_required(VERSION 2.8.8)
project(im4_im5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

set(SOURCE_FILES
    main.cpp
)

add_executable(im4_im5 ${SOURCE_FILES})

