cmake_minimum_required(VERSION 3.10)

project(SimuladorQCL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SimuladorQCL
    src/main.cpp
    src/LaserQCL.cpp
    src/CSVReader.cpp
    src/Materials.cpp
    src/Methods/Leapfrog.cpp
)

add_executable(LeapfrogTest
    test/LeapfrogTest.cpp
    src/Methods/Leapfrog.cpp
)