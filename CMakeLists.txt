cmake_minimum_required(VERSION 3.20)

project(jit VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra")

add_subdirectory(ir)
add_subdirectory(util)
add_subdirectory(examples)
