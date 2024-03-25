
#
# Define the Library
#

add_library(pi-containers INTERFACE)
add_library(pi::containers ALIAS pi-containers)

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
