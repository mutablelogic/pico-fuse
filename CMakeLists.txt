cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(lib/pico-sdk/pico_sdk_init.cmake)

project(picofuse CXX C ASM)

pico_sdk_init()

add_subdirectory(src/fuse)
add_subdirectory(src/rp2040)
add_subdirectory(examples/fusetest)
