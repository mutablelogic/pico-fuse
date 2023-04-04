add_executable(runloop 
    main.c
    runloop.c
    hashmap.c
)

target_include_directories(runloop PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(runloop
    pico_stdlib
    hardware_adc
)

target_compile_definitions(runloop PRIVATE PICO_DEBUG_MALLOC=1)

pico_enable_stdio_usb(runloop 1)
pico_enable_stdio_uart(runloop 0)
pico_add_extra_outputs(runloop)

