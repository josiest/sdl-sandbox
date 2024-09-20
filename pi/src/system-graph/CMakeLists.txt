
#
# Define the Library
#

add_library(pi-system-graph INTERFACE)
add_library(pi::system-graph ALIAS pi-system-graph)

set_target_properties(pi-system-graph PROPERTIES
        CXX_STANDARD 23
        CX_STANDARD_REQUIRED True
        VERSION 0.1.0
        SOVERSION 0)

find_package(EnTT REQUIRED)
target_link_libraries(pi-system-graph INTERFACE pi::graphs EnTT::EnTT)

# specify include directories for pi-system-graph
target_include_directories(
        pi-system-graph INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

#
# Install the Library
#

# install the compiled objects for pi-system-graph
install(TARGETS pi-system-graph
        EXPORT pi-system-graph-targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/system-graph)

# install the include files for pi-system-graph
install(FILES ${CMAKE_SOURCE_DIR}/include/pi/system-graph/system_graph.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/system-graph)

# create and install the target files
install(EXPORT pi-system-graph-targets
        FILE pi-system-graph-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/system-graph)

# generate version for config file
include(CMakePackageConfigHelpers)
get_target_property(pi_system_graph_VERSION pi-system-graph VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pi-system-graph-config-version.cmake"
        VERSION ${pi_system_graph_VERSION}
        COMPATIBILITY AnyNewerVersion)

#install config files
install(FILES pi-system-graph-config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/pi-system-graph-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/system-graph)
