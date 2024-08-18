

set(NAME "helloworld")
add_executable(${NAME}
    helloworld/helloworld.c
)
target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
target_link_libraries(${NAME} picofuse)
pico_add_extra_outputs(${NAME})

