
#
# Define the Library
#

add_library(pi-events STATIC event_sink.cpp)
add_library(pi::events ALIAS pi-events)

set_target_properties(pi-events PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED True
    VERSION 0.1.0
    SOVERSION 0)

# specify include directories for pi-events
target_include_directories(
        pi-events PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(EnTT REQUIRED)

if (TARGET SDL2::SDL2main)
    target_link_libraries(pi-events PUBLIC SDL2main)
endif ()
# link the required dependencies
target_link_libraries(pi-events PUBLIC EnTT::EnTT SDL2::SDL2)

#
# Install the Library
#

# install the compiled objects for pi-events
install(TARGETS pi-events
        EXPORT pi-events-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/pi/events
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/events)

# install the include files for pi-events
install(FILES ${CMAKE_SOURCE_DIR}/include/pi/events/event_sink.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/events)

# create and install the target files
install(EXPORT pi-events-targets
        FILE pi-events-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/events)

# generate version for config file
include(CMakePackageConfigHelpers)
get_target_property(pi_events_VERSION pi-events VERSION)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pi-events-config-version.cmake"
        VERSION ${pi_events_VERSION}
        COMPATIBILITY AnyNewerVersion)

#install config files
install(FILES pi-events-config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/pi-events-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/events)
