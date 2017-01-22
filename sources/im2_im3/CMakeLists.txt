cmake_minimum_required(VERSION 2.8.8)
project(im2_im3)

set(SOURCE_FILES
    main.cpp
    dvyyaz.cpp
)

add_executable(im2_im3 ${SOURCE_FILES} ${BACKWARD_ENABLE})
target_link_libraries(im2_im3 ${COMMON_CXX_LIBS})
