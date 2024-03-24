
#
# Define the Library
#

add_library(pi-events STATIC event_sink.cpp)

# specify include directories for pi-events
target_include_directories(
        pi-events PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies
target_link_libraries(pi-events PUBLIC
        # mingw needs extra linkage for sdl
        $<$<BOOL:MINGW>:mingw32 SDL2main>
        EnTT::EnTT SDL2)

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
