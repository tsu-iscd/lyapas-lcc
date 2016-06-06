cmake_minimum_required(VERSION 2.8.8)
project(lyapas_im0)

find_package(FLEX)
FLEX_TARGET(MyFlex lyapas_im0.l ${CMAKE_BINARY_DIR}/lyapas_im0.c)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(SOURCE_FILES lyapas_im0.c)

add_executable(lyapas_im0 ${SOURCE_FILES})

target_link_libraries(lyapas_im0 "-lfl")
