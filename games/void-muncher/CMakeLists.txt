cmake_minimum_required(VERSION 3.18)
project(void-muncher)

find_package(yaml-cpp REQUIRED)
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(EnTT REQUIRED)

list(APPEND CMAKE_PREFIX_PATH ../lib/cmake)
find_package(pi REQUIRED CONFIG
             REQUIRED COMPONENTS
                graphs containers reflection events
                system-graph yaml-config sdl-systems)
find_package(ImGui REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/basic-components)
add_subdirectory(src/world-system)
add_subdirectory(src/input/keyboard-axis)
add_subdirectory(src/colored-squares)
add_subdirectory(src/basic-movement)

add_executable(void-muncher src/void_muncher.cpp src/void-muncher/munchable_system.cpp
                            src/void-muncher/player_controller.cpp)

target_include_directories(void-muncher PRIVATE ../include include)

if (TARGET SDL2::SDL2main)
    target_link_libraries(void-muncher PRIVATE SDL2main)
endif ()
target_link_libraries(void-muncher PRIVATE
        SDL2::SDL2 EnTT::EnTT yaml-cpp::yaml-cpp ImGui::ImGui
        pi::events pi::system-graph pi::yaml-config pi::sdl-systems
        world-system basic-components keyboard-axis colored-squares basic-movement)
