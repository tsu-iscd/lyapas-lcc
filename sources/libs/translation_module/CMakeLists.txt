cmake_minimum_required(VERSION 2.8.8)
project(translation_module)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/translation_module)
set(TRANSLATION_MODULE_SRCS
    ${PROJECT_SOURCE_DIR}/translation_module.cpp
)

add_library(translation_module STATIC ${TRANSLATION_MODULE_SRCS})
target_link_libraries(translation_module jsoncpp)
target_include_directories(translation_module INTERFACE "${PROJECT_INCLUDE_DIR}")
