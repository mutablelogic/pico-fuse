set(NAME "picofuse")
add_library(${NAME} STATIC
    gpio.c
    led.c
    picofuse.c
    spi.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
    ${CMAKE_CURRENT_LIST_DIR}/../fuse
)

target_link_libraries(${NAME} 
    fuse
)

if(${TARGET_OS} STREQUAL "pico")
    target_link_libraries(${NAME}
        hardware_adc        
        hardware_spi
        pico_stdlib
    )
    if(PICO_CYW43_SUPPORTED)
        target_link_libraries(${NAME}
            pico_cyw43_arch_lwip_threadsafe_background
            pico_lwip_mqtt
        )
        target_compile_definitions(${NAME} PRIVATE PICO_CYW43_SUPPORTED=1)
    endif()
endif()
