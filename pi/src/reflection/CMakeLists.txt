
#
# Define the Library
#

add_library(pi-reflection INTERFACE)
add_library(pi::reflection ALIAS pi-reflection)

set_target_properties(pi-reflection PROPERTIES
        CXX_STANDARD 23
        CX_STANDARD_REQUIRED True
        VERSION 0.1.0
        SOVERSION 0)

# specify include directories for pi-reflection
target_include_directories(
        pi-reflection INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(EnTT REQUIRED)

if (TARGET SDL2::SDL2main)
    target_link_libraries(pi-reflection INTERFACE SDL2main)
endif ()
target_link_libraries(pi-reflection INTERFACE SDL2::SDL2 EnTT::EnTT)

#
# Install the Library
#

# install the compiled objects for pi-reflection
install(TARGETS pi-reflection
        EXPORT pi-reflection-targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/reflection)

# install the include files for pi-reflection
install(FILES
            ${CMAKE_SOURCE_DIR}/include/pi/reflection/reflect.hpp
            ${CMAKE_SOURCE_DIR}/include/pi/reflection/color.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/reflection)

# create and install the target files
install(EXPORT pi-reflection-targets
        FILE pi-reflection-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/reflection)

# generate version for config file
include(CMakePackageConfigHelpers)
get_target_property(pi_reflection_VERSION pi-reflection VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pi-reflection-config-version.cmake"
        VERSION ${pi_reflection_VERSION}
        COMPATIBILITY AnyNewerVersion)

#install config files
install(FILES pi-reflection-config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/pi-reflection-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/reflection)
