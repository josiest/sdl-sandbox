add_library(basic-components INTERFACE)
target_sources(basic-components INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}/include/basic-components
        FILES ${CMAKE_SOURCE_DIR}/include/basic-components/basic_components.hpp)
target_include_directories(basic-components INTERFACE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(basic-components INTERFACE SDL2::SDL2)

