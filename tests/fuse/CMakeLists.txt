
##########################################################################################

set(NAME "test_base64")
add_executable(${NAME} 
    base64/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_event")
add_executable(${NAME} 
    event/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_ftostr")
add_executable(${NAME} 
    ftostr/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_itostr")
add_executable(${NAME} 
    itostr/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_list")
add_executable(${NAME} 
    list/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_mutex")
add_executable(${NAME} 
    mutex/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_panic")
add_executable(${NAME} 
    panic/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL TRUE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_printf")
add_executable(${NAME} 
    printf/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_random")
add_executable(${NAME} 
    random/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_strtostr")
add_executable(${NAME} 
    strtostr/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_timer")
add_executable(${NAME} 
    common/main.c
    timer/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

if(TARGET_OS STREQUAL "pico")
    # enable usb output, disable uart output
    pico_enable_stdio_usb(${NAME} 1)
    pico_enable_stdio_uart(${NAME} 0)
    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${NAME})
endif()

##########################################################################################

set(NAME "test_utostr")
add_executable(${NAME} 
    utostr/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_value")
add_executable(${NAME} 
    value/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)
