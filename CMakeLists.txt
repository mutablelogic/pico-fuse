# Versions
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)

# PICO_BOARD
if (DEFINED ENV{PICO_BOARD})
    set(PICO_BOARD "$ENV{PICO_BOARD}")
    set(TARGET_OS "pico")    
    add_compile_definitions(TARGET_PICO)
    include(lib/pico-sdk/pico_sdk_init.cmake)
else()
    set(TARGET_OS ${CMAKE_HOST_SYSTEM_NAME})
    string(TOLOWER ${TARGET_OS} TARGET_OS)
endif()
message("Target ${TARGET_OS}")

# Define project
project(picofuse CXX C ASM)

# TODO!!!
# Allow use of POSIX extensions
add_compile_definitions(_POSIX_C_SOURCE=200809L)
add_compile_definitions(DEBUG)

# picofuse libraries
add_subdirectory(src/fuse)
if(TARGET_OS STREQUAL "pico")
    pico_sdk_init()
    add_subdirectory(src/picofuse)
endif()

# Tests
include(CTest)
add_subdirectory(tests/fuse)
#add_subdirectory(tests/fuse)
#add_subdirectory(tests/panic)
#add_subdirectory(tests/random)

# Examples
add_subdirectory(examples/fuse)
if(TARGET_OS STREQUAL "pico")
    add_subdirectory(examples/picofuse)
endif()
