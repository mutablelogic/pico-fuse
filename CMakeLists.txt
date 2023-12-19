# Versions
cmake_minimum_required(VERSION 3.12)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# PICO_BOARD
if (DEFINED ENV{PICO_BOARD})
    set(PICO_BOARD $ENV{PICO_BOARD})
    message("Using PICO_BOARD from environment ('${PICO_BOARD}')")
else()
    if (NOT PICO_BOARD)
        set(PICO_BOARD "pico")
        message("Defaulting PICO target board to ${PICO_BOARD} since not specified")
    else()
        message("PICO target board is ${PICO_BOARD}")
    endif()    
endif()

# TARGET set to linux, darwin or pico depending on PICO_BOARD
if(PICO_BOARD STREQUAL "linux")
    set(TARGET "linux")
    add_compile_definitions(TARGET_LINUX)
    add_compile_definitions(TARGET_POSIX)
elseif(PICO_BOARD STREQUAL "darwin")
    set(TARGET "darwin")
    add_compile_definitions(TARGET_DARWIN)
    add_compile_definitions(TARGET_POSIX)
else()
    set(TARGET "pico")
    add_compile_definitions(TARGET_PICO)
    include(lib/pico-sdk/pico_sdk_init.cmake)
endif()

# Define project
project(fuse CXX C ASM)

# TODO!!!
add_compile_definitions(DEBUG)

# Pico libraries
if(${TARGET} STREQUAL "pico")
    pico_sdk_init()
endif()

# Libraries
add_subdirectory(src/alloc)
#add_subdirectory(src/fuse)
#if(${TARGET} STREQUAL "pico")
#    add_subdirectory(src/picofuse)
#endif()

# Tests
include(CTest)

# Tests - alloc
add_subdirectory(tests/alloc)
add_subdirectory(tests/fuse)

# Tests - fuse
#add_subdirectory(tests/app)
#add_subdirectory(tests/list)
#add_subdirectory(tests/map)
#add_subdirectory(tests/panic)
#add_subdirectory(tests/pool)
#add_subdirectory(tests/random)
#add_subdirectory(tests/value)

# Tests - picofuse
#if(${TARGET} STREQUAL "pico")
#    add_subdirectory(tests/blink)
#    add_subdirectory(tests/temperature)
#endif()

