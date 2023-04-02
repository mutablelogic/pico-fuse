add_executable(blink blink.c)

target_link_libraries(blink 
    pico_stdlib
    pico_cyw43_arch_none
)

pico_add_extra_outputs(blink)

