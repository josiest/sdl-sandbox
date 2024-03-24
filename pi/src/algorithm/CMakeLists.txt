
#
# Define the Library
#

add_library(pi-algorithm INTERFACE)
target_link_libraries(pi-algorithm INTERFACE # mingw needs extra linkage for sdl
                                             $<$<BOOL:MINGW>:mingw32 SDL2main>)

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
