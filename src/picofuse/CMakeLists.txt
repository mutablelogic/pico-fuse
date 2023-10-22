set(NAME "picofuse")
add_library(${NAME} STATIC
    led.c
    main.c
    stdio.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(${NAME} 
    fuse
)

if(${TARGET} STREQUAL "pico")
    target_link_libraries(${NAME}
        pico_stdlib
    )
endif()

if(PICO_CYW43_SUPPORTED)
    target_link_libraries(${NAME}
        pico_cyw43_arch_lwip_threadsafe_background
    )
    target_compile_definitions(${NAME} PRIVATE PICO_CYW43_SUPPORTED=1)
endif()
