add_executable(inkypack
    main.c
)

target_include_directories(inkypack PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(inkypack
    pimoroni
)

target_compile_definitions(inkypack PRIVATE PICO_DEBUG_MALLOC=1)

pico_enable_stdio_usb(inkypack 1)
pico_enable_stdio_uart(inkypack 0)
pico_add_extra_outputs(inkypack)
