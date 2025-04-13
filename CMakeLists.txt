cmake_minimum_required(VERSION 3.27)
project(GoStringLogic)

set(CMAKE_CXX_STANDARD 17)

add_executable(GoStringLogic main.cpp
        src/headers/go_logic.h
        src/sources/go_point.cpp
        src/headers/go_point.h
        src/sources/go_logic.cpp
        src/headers/go_string.h
        src/sources/go_string.cpp
        src/headers/go_color_enum.h)
