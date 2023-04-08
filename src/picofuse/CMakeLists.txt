add_library(pico_fuse STATIC
    picofuse.c
    hashmap.c
    ev.c
    ev_init.c
    ev_quit.c
    ev_gpio.c
    ev_led.c
    ev_timer.c
    ev_wifi.c
)

target_include_directories(pico_fuse PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(pico_fuse
    pico_stdlib
    pico_unique_id
)

if(PICO_CYW43_SUPPORTED)
target_link_libraries(pico_fuse
    pico_cyw43_arch_none  
    pico_cyw43_arch_lwip_threadsafe_background
)
endif()

target_compile_definitions(pico_fuse PRIVATE PICO_DEBUG_MALLOC=1)
if(PICO_CYW43_SUPPORTED)
    target_compile_definitions(pico_fuse PRIVATE PICO_CYW43_SUPPORTED=1)
endif()
