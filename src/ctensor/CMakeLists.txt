set(NAME "ctensor")
add_library(${NAME} STATIC
    tensor.c
)

target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
