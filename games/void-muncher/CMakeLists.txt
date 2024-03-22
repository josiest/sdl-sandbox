cmake_minimum_required(VERSION 3.18)
project(void-muncher)

find_package(yaml-cpp REQUIRED)
find_package(SDL2 REQUIRED)
find_package(EnTT REQUIRED)

add_library(world-system STATIC src/game/world_system.cpp)
target_include_directories(world-system PRIVATE include ../../include)
target_link_libraries(world-system PRIVATE
        # mingw needs extra linkage for sdl
        $<$<BOOL:MINGW>:mingw32 SDL2main>
        EnTT::EnTT SDL2)

add_library(keyboard-axis STATIC src/input/keyboard_axis.cpp)
target_include_directories(keyboard-axis PRIVATE include ../../include)
target_link_libraries(keyboard-axis PRIVATE
        # mingw needs extra linkage for sdl
        $<$<BOOL:MINGW>:mingw32 SDL2main>
        EnTT::EnTT SDL2)

add_library(colored-squares STATIC src/visuals/colored_square.cpp)
target_include_directories(colored-squares PRIVATE include ../../include)
target_link_libraries(colored-squares PRIVATE
        # mingw needs extra linkage for sdl
        $<$<BOOL:MINGW>:mingw32 SDL2main>
        EnTT::EnTT SDL2)

add_library(basic-movement STATIC src/mechanics/movement.cpp)
target_include_directories(basic-movement PRIVATE include ../../include)
target_link_libraries(basic-movement PRIVATE
        # mingw needs extra linkage for sdl
        $<$<BOOL:MINGW>:mingw32 SDL2main>
        EnTT::EnTT SDL2)

add_executable(void-muncher src/void_muncher.cpp src/game/munchable_system.cpp
                            src/mechanics/player_controller.cpp)

set_target_properties(
        void-muncher world-system keyboard-axis colored-squares basic-movement
        PROPERTIES
        CXX_STANDARD 23 CXX_STANDARD_REQUIRED TRUE)

target_include_directories(void-muncher PRIVATE ../../include include)

target_link_libraries(void-muncher PRIVATE
        # mingw needs extra linkage for sdl
        $<$<BOOL:MINGW>:mingw32 SDL2main>
        SDL2 EnTT::EnTT yaml-cpp
        world-system keyboard-axis colored-squares basic-movement)
