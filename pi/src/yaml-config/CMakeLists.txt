
#
# Define the Library
#

find_package(yaml-cpp REQUIRED)
add_library(pi-yaml-config STATIC errors.cpp)

# specify include directories for pi-yaml-config
target_include_directories(
        pi-yaml-config PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# link the required dependencies
target_link_libraries(pi-yaml-config PUBLIC # mingw needs extra linkage for sdl
                                            $<$<BOOL:MINGW>:mingw32 SDL2main>
                                            SDL2 yaml-cpp)

#
# Install the Library
#

# install the compiled objects for pi-yaml-config
install(TARGETS pi-yaml-config
        EXPORT pi-yaml-config-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/pi/yaml-config
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/yaml-config)

# install the include files for pi-yaml-config
install(FILES ${CMAKE_SOURCE_DIR}/include/pi/yaml-config/errors.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/yaml-config)

# create and install the target files
install(EXPORT pi-yaml-config-targets
        FILE pi-yaml-config-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/yaml-config)