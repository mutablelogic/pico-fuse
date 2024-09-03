set(NAME "fuse")
add_library(${NAME} STATIC
    alloc.c
    alloc_builtin.c
    base64.c
    data.c
    device.c
    event.c
    ftostr.c
    fuse.c
    itostr.c
    list.c
    map.c
    mutex_pico.c
    mutex_posix.c
    number.c
    null.c
    panic.c
    printf.c
    random_pico.c
    random_posix.c
    sleep_posix.c
    string.c
    strtostr.c
    timer_darwin.c
    timer_linux.c
    timer_pico.c
    value.c
    vtostr.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

if(TARGET_OS STREQUAL "pico")
    target_link_libraries(${NAME}
        pico_rand
        pico_stdlib
        pico_time
    )
endif()
