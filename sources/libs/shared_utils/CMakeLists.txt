cmake_minimum_required(VERSION 2.8.8)
project(shared_utils)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/shared_utils)
set(SHARED_UTILS_SRCS
    ${PROJECT_SOURCE_DIR}/json_helper.cpp
    ${PROJECT_SOURCE_DIR}/array_index.cpp
    ${PROJECT_SOURCE_DIR}/is_int.cpp
)

add_library(shared_utils STATIC ${SHARED_UTILS_SRCS})
target_link_libraries(shared_utils jsoncpp)
target_include_directories(shared_utils INTERFACE "${PROJECT_INCLUDE_DIR}")
