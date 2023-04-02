add_executable(uart 
    main.c
)

target_include_directories(uart PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(uart
    pico_stdlib
    hardware_uart
)

pico_enable_stdio_usb(uart 1)
pico_enable_stdio_uart(uart 0)
pico_add_extra_outputs(uart)

