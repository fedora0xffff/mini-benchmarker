cmake_minimum_required(VERSION 3.10)
project(mini-bench VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
    demo/demo.cpp
    src/benchmark.cpp
    src/bench_data.cpp
)
