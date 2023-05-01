add_library(rp2040 STATIC
    gpio.c
    led.c
    pwm.c
    spi.c
    timer.c
)

target_include_directories(rp2040 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(rp2040
    pico_stdlib
    hardware_pwm
    fuse
)
