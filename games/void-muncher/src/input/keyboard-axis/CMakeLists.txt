add_library(keyboard-axis STATIC keyboard_axis.cpp)
target_sources(keyboard-axis PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include/input/keyboard-axis
    FILES
        ${CMAKE_SOURCE_DIR}/include/input/keyboard-axis/axis2d.hpp
        ${CMAKE_SOURCE_DIR}/include/input/keyboard-axis/keyboard_axis.hpp)

target_include_directories(keyboard-axis PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/../include)
target_link_libraries(keyboard-axis PRIVATE SDL2::SDL2 EnTT::EnTT)

