cmake_minimum_required(VERSION 2.8.8)
project(im1_im2)

include_directories(libraries/jsoncpp)

set(SOURCE_FILES
    libraries/jsoncpp/jsoncpp.cpp
    libraries/backward-cpp/backward.cpp
    main.cpp
    source/CompositeCmd/ICmd.cpp
    source/CompositeCmd/ICmd.h
    source/TreeParser/IParser.cpp
    source/TreeParser/IParser.h
    source/CompositeCmd/LeafCmd.cpp
    source/CompositeCmd/LeafCmd.h
    source/CompositeCmd/CompositeCmd.cpp
    source/CompositeCmd/CompositeCmd.h
    source/CompositeCmd/Composites/ProcedureCmd.cpp
    source/CompositeCmd/Composites/ProcedureCmd.h
    source/TreeParser/CmdFactory.cpp
    source/TreeParser/CmdFactory.h
    source/TreeParser/TreeParser.cpp
    source/TreeParser/TreeParser.h
    source/CompositeCmd/Composites/OperationCmd.cpp
    source/CompositeCmd/Composites/OperationCmd.h
    source/CompositeCmd/Composites/ParagraphCmd.cpp
    source/CompositeCmd/Composites/ParagraphCmd.h
    source/CompositeCmd/Leafs/VariableCmd.cpp
    source/CompositeCmd/Leafs/VariableCmd.h
    source/CompositeCmd/Leafs/ConstantCmd.cpp
    source/CompositeCmd/Leafs/ConstantCmd.h
    source/CompositeCmd/Composites/ComplexCmd.cpp
    source/CompositeCmd/Composites/ComplexCmd.h
    source/CompositeCmd/Composites/PrefaceCmd.cpp
    source/CompositeCmd/Composites/PrefaceCmd.h
    source/CompositeCmd/Composites/ProgramCmd.cpp
    source/CompositeCmd/Composites/ProgramCmd.h
    source/CompositeCmd/Composites/ArgsCmd.cpp
    source/CompositeCmd/Composites/ArgsCmd.h
    source/CompositeCmd/Leafs/StringCmd.cpp
    source/CompositeCmd/Leafs/StringCmd.h
    source/CompositeCmd/Leafs/AsmCmd.cpp
    source/CompositeCmd/Leafs/AsmCmd.h
)

add_executable(im1_im2 ${SOURCE_FILES})
target_link_libraries(im1_im2 "-ldw")
