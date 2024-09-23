add_library(pi-sdl-resources STATIC sdl_handle.cpp sdl_params.cpp sdl_deleter.cpp
                                    window_params.cpp renderer_params.cpp)

add_library(pi::sdl-resources ALIAS pi-sdl-resources)

set_target_properties(pi-sdl-resources PROPERTIES
        CXX_STANDARD 23
        CX_STANDARD_REQUIRED True
        VERSION 0.1.0
        SOVERSION 0)

# specify include directories for pi-sdl-resources
target_include_directories(
        pi-sdl-resources PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(yaml-cpp REQUIRED)
find_package(EnTT REQUIRED)

# link the required dependencies
if (TARGET SDL2::SDL2main)
    target_link_libraries(pi-sdl-resources PUBLIC SDL2main)
endif ()
target_link_libraries(pi-sdl-resources PUBLIC
                      EnTT::EnTT SDL2::SDL2 yaml-cpp::yaml-cpp pi::yaml-config)

#
# Install the Library
#

# install the compiled objects for pi-sdl-resources
install(TARGETS pi-sdl-resources
        EXPORT pi-sdl-resources-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/pi/sdl-resources
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/sdl-resources)

# install the include files for pi-sdl-resources
install(FILES ${CMAKE_SOURCE_DIR}/include/pi/sdl-resources/sdl_handle.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-resources/sdl_params.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-resources/sdl_deleter.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-resources/window_params.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-resources/renderer_params.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/sdl-resources)

# create and install the target files
install(EXPORT pi-sdl-resources-targets
        FILE pi-sdl-resources-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/sdl-resources)

# generate version for config file
include(CMakePackageConfigHelpers)
get_target_property(pi_sdl_resources_VERSION pi-sdl-resources VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pi-sdl-resources-config-version.cmake"
        VERSION ${pi_sdl_resources_VERSION}
        COMPATIBILITY AnyNewerVersion)

#install config files
install(FILES pi-sdl-resources-config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/pi-sdl-resources-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/sdl-resources)
