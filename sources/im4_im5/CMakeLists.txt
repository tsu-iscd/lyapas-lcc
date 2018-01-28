cmake_minimum_required(VERSION 2.8.8)
project(im4_im5)

set(SOURCE_FILES
    main.cpp
    complexoyaz.cpp
    labels_counter.cpp
    variables_counter.cpp
    functional_replacers.cpp
    free_label_replacer.cpp
    free_var_replacer.cpp
)

add_executable(im4_im5 ${SOURCE_FILES} ${BACKWARD_ENABLE})
target_link_libraries(im4_im5 ${COMMON_CXX_LIBS})

