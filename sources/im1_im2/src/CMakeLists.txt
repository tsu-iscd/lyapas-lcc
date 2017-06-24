cmake_minimum_required(VERSION 2.8.8)
project(${BIN_NAME})

add_library(${LIB_NAME} STATIC
    CompositeCmd/ICmd.cpp
    CompositeCmd/ICmd.h
    TreeParser/IParser.cpp
    TreeParser/IParser.h
    CompositeCmd/LeafCmd.cpp
    CompositeCmd/LeafCmd.h
    CompositeCmd/CompositeCmd.cpp
    CompositeCmd/CompositeCmd.h
    CompositeCmd/Composites/ProcedureCmd.cpp
    CompositeCmd/Composites/ProcedureCmd.h
    TreeParser/CmdFactory.cpp
    TreeParser/CmdFactory.h
    TreeParser/TreeParser.cpp
    TreeParser/TreeParser.h
    CompositeCmd/Composites/OperationCmd.cpp
    CompositeCmd/Composites/OperationCmd.h
    CompositeCmd/Composites/ParagraphCmd.cpp
    CompositeCmd/Composites/ParagraphCmd.h
    CompositeCmd/Leafs/VariableCmd.cpp
    CompositeCmd/Leafs/VariableCmd.h
    CompositeCmd/Leafs/ConstantCmd.cpp
    CompositeCmd/Leafs/ConstantCmd.h
    CompositeCmd/Composites/ComplexCmd.cpp
    CompositeCmd/Composites/ComplexCmd.h
    CompositeCmd/Composites/PrefaceCmd.cpp
    CompositeCmd/Composites/PrefaceCmd.h
    CompositeCmd/Composites/ProgramCmd.cpp
    CompositeCmd/Composites/ProgramCmd.h
    CompositeCmd/Composites/ArgsCmd.cpp
    CompositeCmd/Composites/ArgsCmd.h
    CompositeCmd/Leafs/StringCmd.cpp
    CompositeCmd/Leafs/StringCmd.h
    CompositeCmd/Leafs/AsmCmd.cpp
    CompositeCmd/Leafs/AsmCmd.h
)

target_link_libraries(${LIB_NAME} ${COMMON_CXX_LIBS})
set_target_properties(${LIB_NAME} PROPERTIES
    INTERDACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME}
    main.cpp
    ${BACKWARD_ENABLE})
target_link_libraries(${PROJECT_NAME}
    ${LIB_NAME}
)
