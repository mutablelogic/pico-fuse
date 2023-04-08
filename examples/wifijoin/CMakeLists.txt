add_executable(wifijoin 
    main.c
)

target_include_directories(wifijoin PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(wifijoin 
    pico_fuse
)

target_compile_definitions(wifijoin PRIVATE PICO_DEBUG_MALLOC=0)

pico_enable_stdio_usb(wifijoin 1)
pico_enable_stdio_uart(wifijoin 0)
pico_add_extra_outputs(wifijoin)
