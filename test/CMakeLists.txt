cmake_minimum_required(VERSION 3.14)
project(dateTest VERSION 1.0)

add_executable(
    ${PROJECT_NAME}
    dateTest.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    date
    gtest_main
)

add_test(NAME dateTest COMMAND ${PROJECT_NAME})

# post build command
add_custom_command(
     TARGET ${PROJECT_NAME}  
     COMMENT "Run tests"
     POST_BUILD 
     COMMAND ${PROJECT_NAME}
)