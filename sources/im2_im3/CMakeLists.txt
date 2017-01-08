cmake_minimum_required(VERSION 2.8.8)
project(im2_im3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

set(SOURCE_FILES
    main.cpp
    dvyyaz.cpp
)

add_executable(im2_im3 ${SOURCE_FILES})
target_link_libraries(im2_im3 translation_module)
