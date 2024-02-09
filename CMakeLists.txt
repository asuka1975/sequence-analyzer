cmake_minimum_required(VERSION 3.20)

project(sequence-analyzer)

enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_library(sequence-analyzer src/sequence-analyzer.cpp)

target_include_directories(sequence-analyzer PRIVATE $ENV{ASUKA1975_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_directories(sequence-analyzer PRIVATE $ENV{ASUKA1975_LD_LIBRARY_PATH})
target_link_libraries(sequence-analyzer PRIVATE $ENV{ASUKA1975_LD_LIBRARIES})

add_subdirectory(test)