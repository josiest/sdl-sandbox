
#
# Define the Library
#

add_library(pi-algorithm INTERFACE)
add_library(pi::algorithm ALIAS pi-algorithm)

set_target_properties(pi-algorithm PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED True
    VERSION 0.1.0
    SOVERSION 0)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
if (TARGET SDL2::SDL2main)
    target_link_libraries(pi-algorithm INTERFACE SDL2main)
endif ()
target_link_libraries(pi-algorithm INTERFACE SDL2::SDL2)

#
# Install the Library
#

# install the compiled objects for pi-algorithm
install(TARGETS pi-algorithm
        EXPORT pi-algorithm-targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/algorithm)

# install the include files for pi-algorithm
install(FILES
        ${CMAKE_SOURCE_DIR}/include/pi/algorithm/primitives.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/algorithm)

# create and install the target files
install(EXPORT pi-algorithm-targets
        FILE pi-algorithm-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/algorithm)

# generate version for config file
include(CMakePackageConfigHelpers)
get_target_property(pi_algorithm_VERSION pi-algorithm VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pi-algorithm-config-version.cmake"
        VERSION ${pi_algorithm_VERSION}
        COMPATIBILITY AnyNewerVersion)

#install config files
install(FILES pi-algorithm-config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/pi-algorithm-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/algorithm)
