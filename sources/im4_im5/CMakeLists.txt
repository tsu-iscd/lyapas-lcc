cmake_minimum_required(VERSION 2.8.8)
project(im4_im5)

set(SOURCE_FILES
    main.cpp
)

add_executable(im4_im5 ${SOURCE_FILES} ${BACKWARD_ENABLE})
target_link_libraries(im4_im5 translation_module "-ldw")

