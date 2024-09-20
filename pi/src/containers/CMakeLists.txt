
#
# Define the Library
#

add_library(pi-containers INTERFACE)
add_library(pi::containers ALIAS pi-containers)

set_target_properties(pi-containers PROPERTIES
    CXX_STANDARD 23
    CX_STANDARD_REQUIRED True
    VERSION 0.1.0
    SOVERSION 0)

# specify include directories for pi-containers
target_include_directories(
        pi-containers INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

#
# Install the Library
#

# install the compiled objects for pi-containers
install(TARGETS pi-containers
        EXPORT pi-containers-targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/containers)

# install the include files for pi-containers
install(FILES ${CMAKE_SOURCE_DIR}/include/pi/containers/lookup_table.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/containers)

# create and install the target files
install(EXPORT pi-containers-targets
        FILE pi-containers-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/containers)

# generate version for config file
include(CMakePackageConfigHelpers)
get_target_property(pi_containers_VERSION pi-containers VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pi-containers-config-version.cmake"
        VERSION ${pi_containers_VERSION}
        COMPATIBILITY AnyNewerVersion)

#install config files
install(FILES pi-containers-config.cmake
              "${CMAKE_CURRENT_BINARY_DIR}/pi-containers-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/containers)
