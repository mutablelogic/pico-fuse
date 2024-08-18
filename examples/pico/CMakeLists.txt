
##############################################################################

set(NAME "helloworld")

add_executable(${NAME}
    common/main.c
    helloworld/run.c
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

set(NAME "panic")

add_executable(${NAME}
    common/main.c
    panic/run.c
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
    common/main.c
    blink/run.c
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

set(NAME "temp")

add_executable(${NAME}
    common/main.c
    temp/run.c
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

set(NAME "wifi")

add_executable(${NAME}
    common/main.c
    wifi/run.c
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
