cmake_minimum_required(VERSION 3.10)

project(SimuladorQCL)

set(CMAKE_CXX_STANDARD 17)

add_executable(SimuladorQCL
    src/main.cpp
    src/CSVReader.cpp
    src/Material.cpp
    src/MaterialManager.cpp
    src/Methods/Leapfrog.cpp
)