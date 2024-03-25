
#
# Define the Library
#

add_library(pi-system-graph INTERFACE)
add_library(pi::system-graph ALIAS pi-system-graph)
target_link_libraries(pi-system-graph INTERFACE pi::graphs)

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
