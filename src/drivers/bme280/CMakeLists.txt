set(NAME "bme280")
add_library(${NAME} STATIC
    bme280.c
    registers.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../include
    ${CMAKE_CURRENT_LIST_DIR}/../../fuse
)

target_link_libraries(${NAME} 
    picofuse
)
