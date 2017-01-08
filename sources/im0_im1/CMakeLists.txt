cmake_minimum_required(VERSION 2.8.8)
project(im0_im1)

find_package(BISON REQUIRED)
SET(BisonOutput ${PROJECT_BINARY_DIR}/im0_im1.c)
IF(BISON_FOUND)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${BisonOutput}
        COMMAND ${BISON_EXECUTABLE}
                --output=${BisonOutput}
                ${PROJECT_SOURCE_DIR}/im0_im1.y
        COMMENT "Generating im0_im1.c"
    )
ENDIF()

set(SOURCE_FILES
    ${PROJECT_BINARY_DIR}/im0_im1.c
    ${PROJECT_SOURCE_DIR}/print_json.c
)
# для lyapas_im0.c, который генерируется в PROJECT_BINARY_DIR и использует include print_json.h
include_directories(".")

add_executable(im0_im1 ${SOURCE_FILES})
target_link_libraries(im0_im1 "-ljansson")
