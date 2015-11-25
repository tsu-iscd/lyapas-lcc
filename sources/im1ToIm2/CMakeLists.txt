cmake_minimum_required(VERSION 2.8.8)
project(im1ToIm2)

include_directories(libraries/jsoncpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp libraries/jsoncpp/jsoncpp.cpp source/CompositeCmd/ICmd.cpp source/CompositeCmd/ICmd.h source/TreeParser/IParser.cpp source/TreeParser/IParser.h source/CompositeCmd/LeafCmd.cpp source/CompositeCmd/LeafCmd.h source/CompositeCmd/CompositeCmd.cpp source/CompositeCmd/CompositeCmd.h source/CompositeCmd/Compisities/ProcessCmd.cpp source/CompositeCmd/Compisities/ProcessCmd.h source/CompositeCmd/Compisities/StabCompositeCmd.cpp source/CompositeCmd/Compisities/StabCompositeCmd.h source/CompositeCmd/Leafs/StabLeafCmd.cpp source/CompositeCmd/Leafs/StabLeafCmd.h source/TreeParser/CmdFactory.cpp source/TreeParser/CmdFactory.h source/TreeParser/TreeParser.cpp source/TreeParser/TreeParser.h source/CompositeCmd/Compisities/OperationCmd.cpp source/CompositeCmd/Compisities/OperationCmd.h source/CompositeCmd/Compisities/ParagraphCmd.cpp source/CompositeCmd/Compisities/ParagraphCmd.h source/CompositeCmd/Leafs/VariableCmd.cpp source/CompositeCmd/Leafs/VariableCmd.h source/CompositeCmd/Leafs/ConstantCmd.cpp source/CompositeCmd/Leafs/ConstantCmd.h source/CompositeCmd/Compisities/ComplexCmd.cpp source/CompositeCmd/Compisities/ComplexCmd.h source/CompositeCmd/Compisities/PrefaceCmd.cpp source/CompositeCmd/Compisities/PrefaceCmd.h)
add_executable(im1ToIm2 ${SOURCE_FILES})