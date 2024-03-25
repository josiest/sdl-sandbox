add_library(world-system STATIC world_system.cpp)
target_include_directories(world-system PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/../include)
target_sources(world-system PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}/include/world-system
        FILES ${CMAKE_SOURCE_DIR}/include/world-system/world_system.hpp)
target_link_libraries(world-system INTERFACE SDL2::SDL2 EnTT::EnTT pi::system-graph)
