set(NAME "fuse_test")
add_executable(${NAME} 
    list.c
    main.c
    null.c
    scalar.c
)
add_test(NAME ${NAME} COMMAND ${NAME})

target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)
if(${TARGET} STREQUAL "pico")
    target_link_libraries(${NAME} picofuse)
    pico_enable_stdio_usb(${NAME} 1)
    pico_enable_stdio_uart(${NAME} 0)
    pico_add_extra_outputs(${NAME})
endif()
