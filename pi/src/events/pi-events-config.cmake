include(CMakeFindDependencyMacro)
find_dependency(SDL2)
find_dependency(EnTT)
include("${CMAKE_CURRENT_LIST_DIR}/pi-events-targets.cmake")
add_library(pi::events ALIAS pi::pi-events)