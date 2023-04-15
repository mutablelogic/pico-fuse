add_library(rp2040 STATIC
    gpio.c
)

target_include_directories(rp2040 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(rp2040
    pico_stdlib
)
