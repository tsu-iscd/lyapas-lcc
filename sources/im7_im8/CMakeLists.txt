cmake_minimum_required(VERSION 2.8.8)
project(im7_im8)

set(LIB_NAME "${PROJECT_NAME}_lib")
set(BIN_NAME "${PROJECT_NAME}")
set(TEST_NAME "${PROJECT_NAME}_test")

#
# тесты
#
#add_subdirectory(tests)

#
# библиотека
#
set(SOURCE_FILES
    assembler.cpp
)
add_library(${LIB_NAME}
    ${SOURCE_FILES}
)
target_link_libraries(${LIB_NAME}
    ${COMMON_CXX_LIBS}
)
target_include_directories(${LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

#
# приложение
#
add_executable(${BIN_NAME}
    main.cpp
    ${BACKWARD_ENABLE}
)
target_link_libraries(${BIN_NAME}
    ${LIB_NAME}
)
