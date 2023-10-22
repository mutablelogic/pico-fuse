set(NAME pool)
add_executable(${NAME} 
    main.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(${NAME} 
    fuse
)

target_compile_definitions(${NAME} PRIVATE DEBUG=1)

if(${TARGET} STREQUAL "pico")
    pico_add_extra_outputs(${NAME})
endif()
