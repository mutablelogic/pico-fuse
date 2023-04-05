add_library(picofuse SHARED
    picofuse.c
)

target_link_libraries(picofuse
    pico_stdlib
    hardware_adc
)

target_compile_definitions(picofuse PRIVATE PICO_DEBUG_MALLOC=1)
