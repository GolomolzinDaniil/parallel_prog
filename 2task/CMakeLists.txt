cmake_minimum_required(VERSION 3.10)
project(task2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

set(TASKS 1main 2main 3_1main 3_2main)

foreach(TASK ${TASKS})
    add_executable(${TASK} ${TASK}.cpp)

    target_link_libraries(${TASK} PRIVATE OpenMP::OpenMP_CXX)

    target_compile_options(${TASK} PRIVATE -O3)
endforeach()