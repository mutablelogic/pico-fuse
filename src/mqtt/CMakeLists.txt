add_executable(mqtt 
    main.c
    wifi.c
    mqtt.c
)

target_include_directories(mqtt PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(mqtt
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_mqtt
)

pico_enable_stdio_usb(mqtt 1)
pico_enable_stdio_uart(mqtt 0)
pico_add_extra_outputs(mqtt)
