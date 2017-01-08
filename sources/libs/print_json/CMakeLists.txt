cmake_minimum_required(VERSION 2.8.8)
project(print_json)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(PRINT_JSON_SRCS
    ${PROJECT_SOURCE_DIR}/print_json.c
)

add_library(print_json STATIC ${PRINT_JSON_SRCS})
target_include_directories(print_json INTERFACE "${PROJECT_INCLUDE_DIR}")
