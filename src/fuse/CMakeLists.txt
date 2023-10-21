add_library(fuse STATIC
    debug.c
    epoll.c
    fuse.c
    panic.c
    pool.c
    pool_std.c
    sleep.c
    string.c
)

target_include_directories(fuse PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
