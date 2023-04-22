add_executable(picodisplay
    main.c
)

target_include_directories(picodisplay PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(picodisplay
    pimoroni
)

target_compile_definitions(picodisplay PRIVATE PICO_DEBUG_MALLOC=1)

pico_enable_stdio_usb(picodisplay 1)
pico_enable_stdio_uart(picodisplay 0)
pico_add_extra_outputs(picodisplay)
