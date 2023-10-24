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
elseif(PICO_BOARD STREQUAL "darwin")
    set(TARGET "darwin")
else()
    set(TARGET "pico")
    include(lib/pico-sdk/pico_sdk_init.cmake)
endif()

# Define project
project(fuse CXX C ASM)
message("Set TARGET to ${TARGET}")
add_compile_definitions(TARGET=${TARGET})
add_compile_definitions(DEBUG=1)

# Libraries
add_subdirectory(src/fuse)

# Pico libraries
if(${TARGET} STREQUAL "pico")
    pico_sdk_init()
    add_subdirectory(src/picofuse)
endif()

# Tests
add_subdirectory(tests/blink)
add_subdirectory(tests/fuseapp)
add_subdirectory(tests/map)
add_subdirectory(tests/panic)
add_subdirectory(tests/pool)
add_subdirectory(tests/temperature)
