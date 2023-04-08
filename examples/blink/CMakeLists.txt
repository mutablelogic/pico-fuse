add_executable(blink 
    main.c
)

target_include_directories(blink PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_directories(blink PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../picofuse
)

target_link_libraries(blink 
    pico_fuse
)

target_compile_definitions(blink PRIVATE PICO_DEBUG_MALLOC=0)

pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)
pico_add_extra_outputs(blink)
