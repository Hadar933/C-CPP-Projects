cmake_minimum_required(VERSION 3.15)
project(TEST_CPP)

set(CMAKE_CXX_STANDARD 14)

add_executable(TEST_CPP VLVector.hpp test.cpp)