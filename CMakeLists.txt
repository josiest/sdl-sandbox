cmake_minimum_required(VERSION 3.18)
project(sdl-sandbox)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
find_package(EnTT REQUIRED)

add_subdirectory(pi/events)