add_executable(boring-example boring_example.cpp)
set_target_properties(boring-example PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED True)

target_include_directories(boring-example PRIVATE ../include)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

if (TARGET SDL2::SDL2main)
    target_link_libraries(boring-example PRIVATE SDL2::SDL2main)
endif()

list(APPEND CMAKE_MODULE_PATH ../lib/cmake)
find_package(pi COMPONENTS sdl-resources REQUIRED)
target_link_libraries(boring-example PRIVATE SDL2::SDL2 pi::sdl-resources)