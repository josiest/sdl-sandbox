add_library(basic-movement STATIC movement.cpp)
target_sources(basic-movement PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/basic-movement
    FILES ${CMAKE_SOURCE_DIR}/include/basic-movement/movement.hpp)

target_include_directories(basic-movement PRIVATE
        ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/../include)

target_link_libraries(basic-movement PRIVATE
        EnTT::EnTT SDL2::SDL2 basic-components)
