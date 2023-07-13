cmake_minimum_required(VERSION 3.18)
project(void-muncher)

add_executable(void-muncher src/void_muncher.cpp)
set_target_properties(void-muncher PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED TRUE)

target_include_directories(void-muncher PRIVATE ../../include include)

find_package(yaml-cpp REQUIRED)
find_package(SDL2 REQUIRED)
find_package(EnTT REQUIRED)
target_link_libraries(void-muncher PRIVATE SDL2 EnTT::EnTT yaml-cpp)
