set(NAME "picofuse")
add_library(${NAME} STATIC
    adc.c
    gpio.c
    led.c
    mqtt.c
    serial.c
    stdio.c
    wifi.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(${NAME} 
    fuse
)

if(${TARGET_OS} STREQUAL "pico")
    target_link_libraries(${NAME}
        hardware_adc        
        pico_stdlib
        pico_unique_id
    )
endif()

if(PICO_CYW43_SUPPORTED)
    target_link_libraries(${NAME}
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_mqtt
    )
    target_compile_definitions(${NAME} PRIVATE PICO_CYW43_SUPPORTED=1)
endif()
