cmake_minimum_required(VERSION 3.18)
project(regular-polygons)

add_executable(sketch src/sketch.cpp)
set_target_properties(sketch PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

find_package(SDL2 REQUIRED)
target_link_libraries(sketch PRIVATE SDL2)
