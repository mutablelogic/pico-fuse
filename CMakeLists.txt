cmake_minimum_required(VERSION 3.12)

project(fuse CXX C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Define the PICO_BOARD
if (DEFINED ENV{PICO_BOARD})
    set(PICO_BOARD $ENV{PICO_BOARD})
    message("Using PICO_BOARD from environment ('${PICO_BOARD}')")
else()
    if (NOT PICO_BOARD)
        set(PICO_BOARD "pico")
        pico_message("Defaulting PICO target board to ${PICO_BOARD} since not specified.")
    else()
        message("PICO target board is ${PICO_BOARD}.")
    endif()    
endif()

# Define the TARGET as linux, darwin or pico
if(${PICO_BOARD} STREQUAL "linux")
    set(TARGET linux)
    add_compile_definitions(TARGET=linux)
elseif(${PICO_BOARD} STREQUAL "darwin")
    set(TARGET darwin)
    add_compile_definitions(TARGET=darwin)
else()
    message(STATUS "TARGET=pico, ${PICO_BOARD}")
    set(TARGET pico)
    include(lib/pico-sdk/pico_sdk_init.cmake)
    add_compile_definitions(TARGET=pico)
    pico_sdk_init()
endif()

# Libraries
add_subdirectory(src/fuse)

# Tests
add_subdirectory(tests/panic)
add_subdirectory(tests/pool)
add_subdirectory(tests/fuseapp)
