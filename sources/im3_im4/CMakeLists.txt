cmake_minimum_required(VERSION 2.8.8)
project(im3_im4)

set(SOURCE_FILES
    main.cpp
    steckoyaz.cpp
    function.cpp
    function_info.cpp
)

add_executable(im3_im4 ${SOURCE_FILES} ${BACKWARD_ENABLE})
target_link_libraries(im3_im4 ${COMMON_CXX_LIBS})
