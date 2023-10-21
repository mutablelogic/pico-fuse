add_executable(fuseapp 
    main.c
)

target_include_directories(fuseapp PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(fuseapp 
    fuse
)

target_compile_definitions(fuseapp PRIVATE DEBUG=1)
