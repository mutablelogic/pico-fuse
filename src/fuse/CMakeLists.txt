add_library(fuse STATIC
    debug.c
    main.c
    pool.c
)

target_include_directories(fuse PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(fuse
    pico_stdlib
)
