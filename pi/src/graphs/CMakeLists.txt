
#
# Define the Library
#

add_library(pi-graphs INTERFACE)

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
