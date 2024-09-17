cmake_minimum_required(VERSION 3.26)
project(lab1smarter)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_BUILD_TYPE Debug)

add_executable(lab1smarter solution.cpp
        Grad.cpp
        Grad.h
        Mapa.cpp
        Mapa.h
        Algorithms.cpp
        Algorithms.h
)
