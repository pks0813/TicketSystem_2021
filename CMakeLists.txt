cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

#add_definitions(-O2)

add_executable(code
        Storagepool.hpp
        include.hpp
        tools.hpp
        Bpt.hpp
        main.cpp
        corn.hpp)
