cmake_minimum_required(VERSION 2.8.8)
project(${BIN_NAME})

add_library(${LIB_NAME} STATIC
    CompositeCmd/ICmd.cpp
    CompositeCmd/LeafCmd.cpp
    CompositeCmd/CompositeCmd.cpp
    CompositeCmd/Composites/ProcedureCmd.cpp
    TreeParser/CmdFactory.cpp
    TreeParser/TreeParser.cpp
    CompositeCmd/Composites/OperationCmd.cpp
    CompositeCmd/Composites/ParagraphCmd.cpp
    CompositeCmd/Leafs/VariableCmd.cpp
    CompositeCmd/Leafs/ConstantCmd.cpp
    CompositeCmd/Composites/ComplexCmd.cpp
    CompositeCmd/Composites/PrefaceCmd.cpp
    CompositeCmd/Composites/ProgramCmd.cpp
    CompositeCmd/Composites/ArgsCmd.cpp
    CompositeCmd/Leafs/StringCmd.cpp
    CompositeCmd/Leafs/AsmCmd.cpp
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
