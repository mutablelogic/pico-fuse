cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(NOT EXISTS ${PICO_SDK_PATH})
    message(FATAL_ERROR "Set the PICO_SDK_PATH environment variable.")
endif()

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(pico_frame CXX C ASM)

pico_sdk_init()

add_subdirectory(src/blink)
