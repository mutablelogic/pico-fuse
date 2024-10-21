set(NAME "uc8151")
add_library(${NAME} STATIC
    uc8151.c
    readwrite.c
    registers.c
    resetbusy.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../../include
    ${CMAKE_CURRENT_LIST_DIR}/../../fuse
)

target_link_libraries(${NAME} 
    picofuse
)
