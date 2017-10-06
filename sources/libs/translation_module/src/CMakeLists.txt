cmake_minimum_required(VERSION 2.8.8)
project(${LIB_NAME})

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/translation_module)
set(TRANSLATION_MODULE_SRCS
    ${PROJECT_SOURCE_DIR}/translation_module.cpp
    ${PROJECT_SOURCE_DIR}/cmd_info.cpp
)

add_library(${LIB_NAME} STATIC ${TRANSLATION_MODULE_SRCS})
target_link_libraries(${LIB_NAME} shared_utils jsoncpp)
target_include_directories(${LIB_NAME} INTERFACE "${PROJECT_INCLUDE_DIR}")
