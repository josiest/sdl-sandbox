add_library(colored-squares STATIC colored_square.cpp)
target_sources(colored-squares PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_SOURCE_DIR}/include/colored-squares
        FILES
            ${CMAKE_SOURCE_DIR}/include/colored-squares/arithmetic.hpp
            ${CMAKE_SOURCE_DIR}/include/colored-squares/colored_square.hpp)

target_include_directories(colored-squares PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}}/../include)

target_link_libraries(colored-squares PRIVATE
        EnTT::EnTT SDL2::SDL2 basic-components)
