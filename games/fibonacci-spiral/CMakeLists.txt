cmake_minimum_required(VERSION 3.18)
project(fibonacci-spiral)

add_executable(sketch src/sketch.cpp)
set_target_properties(sketch PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

target_include_directories(sketch PRIVATE ../include include)

find_package(yaml-cpp REQUIRED)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(EnTT REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ../lib)
find_package(pi COMPONENTS containers reflection graphs
                           system-graph yaml-config sdl-systems REQUIRED)

if (TARGET SDL2::SDL2main)
    target_link_libraries(sketch PRIVATE SDL2main)
endif()

target_link_libraries(sketch PRIVATE SDL2::SDL2 EnTT::EnTT yaml-cpp::yaml-cpp
                                     pi::system-graph pi::yaml-config pi::sdl-systems)