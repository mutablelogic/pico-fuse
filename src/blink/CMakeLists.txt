add_executable(blink blink.c)

target_include_directories(blink PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../picofuse/include
)

target_link_directories(blink PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../picofuse
)

target_link_libraries(blink 
    pico_fuse
)

pico_add_extra_outputs(blink)
