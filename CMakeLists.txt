cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(fuse CXX C ASM)

if($ENV{PICO_BOARD} STREQUAL "linux")
    message(STATUS "TARGET=linux")
    add_compile_definitions(TARGET=linux)
elseif($ENV{PICO_BOARD} STREQUAL "darwin")
    message(STATUS "TARGET=darwin")
    add_compile_definitions(TARGET=darwin)
else()
    message(STATUS "TARGET=pico, ${PICO_BOARD}")
    add_compile_definitions(TARGET=pico)
    include(lib/pico-sdk/pico_sdk_init.cmake)
    pico_sdk_init()
endif()

# Libraries
add_subdirectory(src/fuse)

# Tests
add_subdirectory(tests/panic)
add_subdirectory(tests/pool)
