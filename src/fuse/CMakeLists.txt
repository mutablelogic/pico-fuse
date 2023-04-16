add_library(fuse STATIC
    action.c
    debug.c
    hashmap.c
    main.c
    pool.c
    queue.c
)

target_include_directories(fuse PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(fuse
    pico_stdlib
)
