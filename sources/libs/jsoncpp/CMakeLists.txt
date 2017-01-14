cmake_minimum_required(VERSION 2.8.8)
project(jsoncpp)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(JSON_CPP_SRCS
    ${PROJECT_SOURCE_DIR}/jsoncpp.cpp
)

add_library(jsoncpp STATIC ${JSON_CPP_SRCS})
target_include_directories(jsoncpp INTERFACE "${PROJECT_INCLUDE_DIR}")