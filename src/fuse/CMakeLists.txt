set(NAME "fuse")
add_library(${NAME} STATIC
    debug.c
    epoll.c
    fuse.c
    map.c
    panic.c
    pool.c
    pool_std.c
    sleep.c
    string.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
