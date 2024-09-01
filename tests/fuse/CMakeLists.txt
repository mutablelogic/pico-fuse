
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

set(NAME "test_ftoa")
add_executable(${NAME} 
    ftoa/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_itoa")
add_executable(${NAME} 
    itoa/main.c
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

set(NAME "test_strtoa")
add_executable(${NAME} 
    strtoa/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_timer")
add_executable(${NAME} 
    timer/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)

##########################################################################################

set(NAME "test_utoa")
add_executable(${NAME} 
    utoa/main.c
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
