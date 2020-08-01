cmake_minimum_required(VERSION 3.15)
project(TEST_C C)

set(CMAKE_C_STANDARD 99)

add_executable(TEST_C SpreaderDetectorBackend.c)