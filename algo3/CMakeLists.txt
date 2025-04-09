cmake_minimum_required(VERSION 3.23)
project(algo3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(tp1)
include_directories(tp2)
include_directories(tp3)

add_executable(algo3
        tp3/ej1.cpp)
