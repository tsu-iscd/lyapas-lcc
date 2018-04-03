cmake_minimum_required(VERSION 2.8.8)
project(${LIB_NAME})

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(ROOT translation_module)

set(SOURCES
    ${ROOT}/cmd_info.cpp
    ${ROOT}/free_label_replacer.cpp
    ${ROOT}/free_var_replacer.cpp
    ${ROOT}/labels_counter.cpp
    ${ROOT}/translation_module.cpp
    ${ROOT}/variables_counter.cpp
)

add_library(${LIB_NAME} STATIC ${SOURCES})
target_link_libraries(${LIB_NAME} shared_utils jsoncpp)
target_include_directories(${LIB_NAME} PRIVATE "${ROOT}")
target_include_directories(${LIB_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
