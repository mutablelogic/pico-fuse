set(NAME "alloc")
add_library(${NAME} STATIC
    alloc.c
    builtin.c
    fuse.c
    panic.c
    sleep_posix.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
