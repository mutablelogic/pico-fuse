set(NAME "fuse")
add_library(${NAME} STATIC
    alloc.c
    alloc_builtin.c
    debug.c
    device.c
    fuse.c
    itoa.c
    list.c
    map.c
    panic.c
    printf.c
    random_pico.c
    random_posix.c
    sleep_posix.c
    strtoa.c
    vtoa.c
)


target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

if(TARGET_OS STREQUAL "pico")
    target_link_libraries(${NAME}
        pico_stdlib
    )
endif()
