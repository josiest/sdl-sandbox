
#
# Define the Library
#

add_library(pi-graphs INTERFACE)
add_library(pi::graphs ALIAS pi-graphs)

set_target_properties(pi-graphs PROPERTIES
        CXX_STANDARD 23
        CX_STANDARD_REQUIRED True
        VERSION 0.1.0
        SOVERSION 0)

# specify include directories for pi-graphs
target_include_directories(
        pi-graphs INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

#
# Install the Library
#

# install the compiled objects for pi-graphs
install(TARGETS pi-graphs
        EXPORT pi-graphs-targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/graphs)

# install the include files for pi-graphs
install(FILES ${CMAKE_SOURCE_DIR}/include/pi/graphs/digraph.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/graphs)

# create and install the target files
install(EXPORT pi-graphs-targets
        FILE pi-graphs-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/graphs)

# generate version for config file
include(CMakePackageConfigHelpers)
get_target_property(pi_graphs_VERSION pi-graphs VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pi-graphs-config-version.cmake"
        VERSION ${pi_graphs_VERSION}
        COMPATIBILITY AnyNewerVersion)

#install config files
install(FILES pi-graphs-config.cmake
              "${CMAKE_CURRENT_BINARY_DIR}/pi-graphs-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/graphs)
