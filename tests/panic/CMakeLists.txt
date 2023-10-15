add_executable(panic 
    main.c
)

target_include_directories(panic PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(panic 
    fuse
)

target_compile_definitions(panic PRIVATE DEBUG=1)
