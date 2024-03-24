
#
# Define the Library
#

add_library(pi-sdl-systems STATIC init_params.cpp init_system.cpp sdl_deleter.cpp
                                  window_params.cpp window_system.cpp
                                  renderer_params.cpp renderer_system.cpp)

# specify include directories for pi-sdl-systems
target_include_directories(
        pi-sdl-systems PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies
target_link_libraries(pi-sdl-systems PUBLIC
        # mingw needs extra linkage for sdl
        $<$<BOOL:MINGW>:mingw32 SDL2main>
        EnTT::EnTT SDL2)

#
# Install the Library
#

# install the compiled objects for pi-sdl-systems
install(TARGETS pi-sdl-systems
        EXPORT pi-sdl-systems-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/pi/sdl-systems
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/sdl-systems)

# install the include files for pi-sdl-systems
install(FILES ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/init_params.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/init_system.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/sdl_deleter.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/window_params.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/window_system.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/renderer_params.hpp
              ${CMAKE_SOURCE_DIR}/include/pi/sdl-systems/renderer_system.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/sdl-systems)

# create and install the target files
install(EXPORT pi-sdl-systems-targets
        FILE pi-sdl-systems-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/sdl-systems)
