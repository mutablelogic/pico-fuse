

set(NAME "helloworld")
add_executable(${NAME}
    helloworld/helloworld.c
)
target_link_libraries(${NAME} pico_stdlib)
pico_add_extra_outputs(${NAME})

