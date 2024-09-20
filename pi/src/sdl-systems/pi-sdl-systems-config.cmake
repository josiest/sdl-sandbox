include(CMakeFindDependencyMacro)
find_dependency(SDL2)
find_dependency(EnTT)
find_dependency(yaml-cpp)
find_dependency(pi-system-graph)
find_dependency(pi-yaml-config)
include("${CMAKE_CURRENT_LIST_DIR}/pi-sdl-systems-targets.cmake")
add_library(pi::sdl-systems ALIAS pi::pi-sdl-systems)