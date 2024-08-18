
##############################################################################

set(NAME "helloworld")

add_executable(${NAME}
    helloworld/helloworld.c
)
target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
target_link_libraries(${NAME} picofuse)

# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${NAME})

##############################################################################

set(NAME "blink")

add_executable(${NAME}
    blink/blink.c
)
target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
target_link_libraries(${NAME} pico_stdlib)

# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${NAME})
