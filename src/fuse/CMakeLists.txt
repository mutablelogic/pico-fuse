add_library(fuse STATIC
    debug.c
    fuse.c
    panic.c
    pool.c
    sleep.c
)

target_include_directories(fuse PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
