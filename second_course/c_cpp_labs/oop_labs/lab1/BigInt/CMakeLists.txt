cmake_minimum_required(VERSION 3.21)
project(untitled6)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES bigint.cpp bigint_test.cpp)
add_executable(BigInt_run ${SOURCE_FILES})

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_link_libraries(BigInt_run gtest gtest_main)