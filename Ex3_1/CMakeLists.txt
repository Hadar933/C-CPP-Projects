cmake_minimum_required(VERSION 3.15)
project(Ex3_1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Ex3_1 RBTree.c Structs.c Structs.h RBTree.h utilities/RBUtilities.h
        utilities/RBUtilities.c tests.c)