
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

set(NAME "test_alloc")
add_executable(${NAME} 
    alloc/main.c
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

set(NAME "test_value")
add_executable(${NAME} 
    value/main.c
)
add_test(NAME ${NAME} COMMAND ${NAME})
set_tests_properties(${NAME} PROPERTIES WILL_FAIL FALSE)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../include)
target_link_libraries(${NAME} fuse)
