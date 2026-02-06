cmake_minimum_required(VERSION 3.10)
project(1task)

add_executable(main main.cpp)

set(CMAKE_CXX_STANDARD 17)


option(USE_DOUBLE "double or float type" OFF)

if (USE_DOUBLE)
    target_compile_definitions(main PRIVATE DOUBLE)
endif()