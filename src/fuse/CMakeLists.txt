set(NAME "fuse")
add_library(${NAME} STATIC
    alloc.c
    alloc_builtin.c
    debug.c
    fuse.c
    magic.c
    panic.c
    random_pico.c
    random_posix.c
    sleep_posix.c
    value.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
