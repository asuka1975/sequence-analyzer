cmake_minimum_required(VERSION 3.20)

project(result_test)

find_package(GTest REQUIRED)

function(add_unittest test_suit)
    message(${test_suit})
    add_executable(${test_suit} ${test_suit}.cpp)
    target_include_directories(${test_suit} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include ${GTEST_INCLUDE_DIRS})
    target_link_libraries(${test_suit} PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
    add_test(NAME run_${test_suit} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${test_suit})
endfunction()
