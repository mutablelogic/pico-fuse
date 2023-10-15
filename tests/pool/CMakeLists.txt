add_executable(pool 
    main.c
)

target_include_directories(pool PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(pool 
    fuse
)

target_compile_definitions(pool PRIVATE DEBUG=1)
