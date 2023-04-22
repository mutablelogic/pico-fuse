add_library(pimoroni STATIC
    picodisplay.c
)

target_include_directories(pimoroni PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
)

target_link_libraries(pimoroni
    fuse
    rp2040
)
