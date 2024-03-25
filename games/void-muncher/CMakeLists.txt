cmake_minimum_required(VERSION 3.18)
project(void-muncher)

find_package(yaml-cpp REQUIRED)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(EnTT REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ../lib/cmake)
find_package(pi COMPONENTS graphs containers reflection events
                           system-graph yaml-config sdl-systems REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/basic-components)
add_subdirectory(src/world-system)

add_library(keyboard-axis STATIC src/input/keyboard_axis.cpp)
target_include_directories(keyboard-axis PRIVATE include ../include)
target_link_libraries(keyboard-axis PRIVATE SDL2::SDL2 EnTT::EnTT)

add_library(colored-squares STATIC src/visuals/colored_square.cpp)
target_include_directories(colored-squares PRIVATE include ../include)
target_link_libraries(colored-squares PRIVATE EnTT::EnTT SDL2::SDL2 basic-components)

add_library(basic-movement STATIC src/mechanics/movement.cpp)
target_include_directories(basic-movement PRIVATE include ../include)
target_link_libraries(basic-movement PRIVATE EnTT::EnTT SDL2::SDL2 basic-components)

add_executable(void-muncher src/void_muncher.cpp src/game/munchable_system.cpp
                            src/mechanics/player_controller.cpp)

target_include_directories(void-muncher PRIVATE ../include include)

if (TARGET SDL2::SDL2main)
    target_link_libraries(void-muncher PRIVATE SDL2main)
endif ()
target_link_libraries(void-muncher PRIVATE
        SDL2::SDL2 EnTT::EnTT yaml-cpp::yaml-cpp
        pi::events pi::system-graph pi::yaml-config pi::sdl-systems
        world-system basic-components keyboard-axis colored-squares basic-movement)
