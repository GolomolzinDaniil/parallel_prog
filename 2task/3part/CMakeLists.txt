cmake_minimum_required(VERSION 3.10)
project(3part LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

option(SND_VERSION "use second file" OFF)
if (SND_VERSION)
    add_executable(main 2main.cpp)
else()
    add_executable(main 1main.cpp)
endif()

target_compile_options(main PRIVATE OpenMP::OpenMP_CXX)
target_link_options(main PRIVATE -O3)
