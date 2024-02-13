cmake_minimum_required(VERSION 3.20)

project(sequence-analyzer_test)

find_package(GTest REQUIRED)

function(add_unittest test_suit)
    message(${test_suit})
    add_executable(${test_suit} ${test_suit}.cpp)
    target_include_directories(${test_suit} PRIVATE $ENV{ASUKA1975_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../include ${GTEST_INCLUDE_DIRS})
    target_link_libraries(${test_suit} PRIVATE $ENV{ASUKA1975_LD_LIBRARIES} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
    target_link_directories(${test_suit} PRIVATE $ENV{ASUKA1975_LD_LIBRARY_PATH})
    add_test(NAME run_${test_suit} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${test_suit})
endfunction()

add_unittest(rule_test)
add_unittest(list_rule_test)
add_unittest(candidate_rule_test)
add_unittest(sequence_rule_test)