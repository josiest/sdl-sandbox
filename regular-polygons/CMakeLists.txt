cmake_minimum_required(VERSION 3.18)
project(regular-polygons)

add_executable(sketch src/sketch.cpp src/poly/system.cpp)
set_target_properties(sketch PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

target_include_directories(sketch PRIVATE ../pi/include include)

find_package(SDL2 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(EnTT REQUIRED)
target_link_libraries(sketch PRIVATE SDL2 yaml-cpp EnTT::EnTT)

target_compile_options(sketch PRIVATE -fconcepts-diagnostics-depth=5)
