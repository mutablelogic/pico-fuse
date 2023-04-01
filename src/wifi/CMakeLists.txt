add_executable(wifi 
    main.c
)

target_include_directories(wifi PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
)

target_link_libraries(wifi
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_mbedtls
    pico_lwip_mqtt
    pico_mbedtls
)

pico_add_extra_outputs(wifi)

