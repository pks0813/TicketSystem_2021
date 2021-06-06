cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-Ofast)

add_executable(code
        map.hpp
        Storagepool.hpp
        include.hpp
        tools.hpp
        BPT.hpp
        main.cpp
        core.hpp)
