cmake_minimum_required(VERSION 3.21)
project(untitled5)

set(CMAKE_CXX_STANDARD 14)

add_executable(BigInt "main.cpp" main.cpp bigint.h bigint.cpp bigint_test.cpp)