
#
# Define the Library
#

add_library(pi-sdl-systems STATIC sdl_system.cpp sdl_deleter.cpp
                                  window_params.cpp window_system.cpp
                                  renderer_params.cpp renderer_system.cpp)

add_library(pi::sdl-systems ALIAS pi-sdl-systems)

set_target_properties(pi-sdl-systems PROPERTIES
        CXX_STANDARD 23
        CX_STANDARD_REQUIRED True
        VERSION 0.1.0
        SOVERSION 0)

# specify include directories for pi-sdl-systems
target_include_directories(
        pi-sdl-systems PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(yaml-cpp REQUIRED)
find_package(EnTT REQUIRED)

# link the required dependencies
if (TARGET SDL2::SDL2main)
    target_link_libraries(pi-sdl-systems PUBLIC SDL2main)
endif ()
target_link_libraries(pi-sdl-systems PUBLIC
        EnTT::EnTT SDL2::SDL2 yaml-cpp::yaml-cpp
        pi::sdl-resources pi::system-graph pi::yaml-config)

#
# Install the Library
#

# install the compiled objects for pi-sdl-systems
install(TARGETS pi-sdl-systems
        EXPORT pi-sdl-systems-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/pi/sdl-systems
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/sdl-systems)

# install the include files for pi-sdl-systems
install(FILES ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/sdl_system.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/sdl_deleter.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/window_params.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/window_system.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/renderer_params.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/renderer_system.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/sdl-systems)

# create and install the target files
install(EXPORT pi-sdl-systems-targets
        FILE pi-sdl-systems-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/sdl-systems)

# generate version for config file
include(CMakePackageConfigHelpers)
get_target_property(pi_sdl_systems_VERSION pi-sdl-systems VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pi-sdl-systems-config-version.cmake"
        VERSION ${pi_sdl_systems_VERSION}
        COMPATIBILITY AnyNewerVersion)

#install config files
install(FILES pi-sdl-systems-config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/pi-sdl-systems-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/sdl-systems)
