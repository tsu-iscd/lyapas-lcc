cmake_minimum_required(VERSION 2.8.8)
project(main)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory("sources/lyapas_im0")
add_subdirectory("sources/im0_im1")
add_subdirectory("sources/im1_im2")
