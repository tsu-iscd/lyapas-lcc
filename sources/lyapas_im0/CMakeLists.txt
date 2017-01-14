cmake_minimum_required(VERSION 2.8.8)
project(lyapas_im0)

find_package(FLEX REQUIRED)
SET(FlexOutput ${PROJECT_BINARY_DIR}/lyapas_im0.c)
IF(FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${FlexOutput}
        COMMAND ${FLEX_EXECUTABLE}
                --outfile=${FlexOutput}
                ${PROJECT_SOURCE_DIR}/lyapas_im0.l
        COMMENT "Generating lyapas_im0.c"
    )
ENDIF()

set(SOURCE_FILES
    ${PROJECT_BINARY_DIR}/lyapas_im0.c
)

add_executable(lyapas_im0 ${SOURCE_FILES})
target_link_libraries(lyapas_im0 ${COMMON_C_LIBS} "-lfl")
