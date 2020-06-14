cmake_minimum_required(VERSION 3.15)
project(Ex4)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ex4 main.cpp
               Activation.h Digit.h Matrix.h MlpNetwork.h Dense.h
               Matrix.cpp Activation.cpp Dense.cpp MlpNetwork.cpp)