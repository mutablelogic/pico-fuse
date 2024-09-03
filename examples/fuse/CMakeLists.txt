
##############################################################################

set(NAME "helloworld")

add_executable(${NAME}
    common/main.c
    helloworld/run.c
)
target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
target_link_libraries(${NAME} fuse)

if(TARGET_OS STREQUAL "pico")
    # enable usb output, disable uart output
    pico_enable_stdio_usb(${NAME} 1)
    pico_enable_stdio_uart(${NAME} 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${NAME})
endif()
