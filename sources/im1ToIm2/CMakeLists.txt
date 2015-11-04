cmake_minimum_required(VERSION 3.3)
project(im1ToIm2)

include_directories(libraries/jsoncpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp libraries/jsoncpp/jsoncpp.cpp source/CompositiesCmd/CompositeCmd.cpp source/CompositiesCmd/CompositeCmd.h source/ParsersTree/ParserTree.cpp source/ParsersTree/ParserTree.h)
add_executable(im1ToIm2 ${SOURCE_FILES})