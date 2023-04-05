add_library(pico_fuse STATIC
    picofuse.c
    ev_init.c
    ev_quit.c
)

target_include_directories(pico_fuse PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(pico_fuse
    pico_stdlib
    hardware_adc
)

target_compile_definitions(pico_fuse PRIVATE PICO_DEBUG_MALLOC=1)
