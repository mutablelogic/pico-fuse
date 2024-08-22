
##############################################################################

set(NAME "helloworld")

add_executable(${NAME}
    common/main.c
    helloworld/run.c
)
target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)
target_link_libraries(${NAME} fuse)
