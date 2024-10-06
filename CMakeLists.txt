# Versions
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)

# PICO_BOARD
if (DEFINED ENV{PICO_BOARD})
    set(PICO_BOARD "$ENV{PICO_BOARD}")
    set(TARGET_OS "pico")    
    include(lib/pico-sdk/pico_sdk_init.cmake)
else()
    set(TARGET_OS ${CMAKE_HOST_SYSTEM_NAME})
    string(TOLOWER ${TARGET_OS} TARGET_OS)
endif()
message("Target ${TARGET_OS}")

# TARGET_DARWIN, TARGET_LINUX and TARGET_PICO
if (TARGET_OS STREQUAL "darwin")
    add_compile_definitions(TARGET_DARWIN)
elseif (TARGET_OS STREQUAL "linux")
    add_compile_definitions(TARGET_LINUX)
elseif (TARGET_OS STREQUAL "pico")
    add_compile_definitions(TARGET_PICO)
endif()

# Define project
project(picofuse CXX C ASM)

# TODO!!!
add_compile_definitions(DEBUG)

# picofuse libraries
add_subdirectory(src/fuse)
if(TARGET_OS STREQUAL "pico")
    pico_sdk_init()
    add_subdirectory(src/picofuse)
    add_subdirectory(src/drivers/bme280)
    add_subdirectory(src/drivers/uc8151)
endif()

# Tests
include(CTest)
add_subdirectory(tests/fuse)

# Examples
if(TARGET_OS STREQUAL "pico")
    add_subdirectory(examples)
endif()
