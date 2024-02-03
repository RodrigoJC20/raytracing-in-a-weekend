cmake_minimum_required(VERSION 3.20)
project(Raytracer VERSION 0.1.0 DESCRIPTION "First attempt of raytracer")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(app main.cpp)
