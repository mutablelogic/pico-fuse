cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(fuse CXX C ASM)

if($ENV{PICO_BOARD} STREQUAL "linux")
    message(STATUS "TARGET=linux")
else()
    message(STATUS "TARGET=pico, ${PICO_BOARD}")
    include(lib/pico-sdk/pico_sdk_init.cmake)
    pico_sdk_init()
endif()

# Add compile definitions
add_compile_definitions(
    TARGET=$ENV{PICO_BOARD}
)

# Libraries
add_subdirectory(src/fuse)

# Tests
add_subdirectory(tests/panic)
add_subdirectory(tests/pool)
