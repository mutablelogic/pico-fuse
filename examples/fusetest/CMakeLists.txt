add_executable(fusetest
    main.c
)

target_include_directories(fusetest PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(fusetest
    fuse
    rp2040
)

target_compile_definitions(fusetest PRIVATE PICO_DEBUG_MALLOC=1)

pico_enable_stdio_usb(fusetest 1)
pico_enable_stdio_uart(fusetest 0)
pico_add_extra_outputs(fusetest)
