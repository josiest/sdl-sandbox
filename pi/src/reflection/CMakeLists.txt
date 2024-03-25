
#
# Define the Library
#

add_library(pi-reflection INTERFACE)
add_library(pi::reflection ALIAS pi-reflection)

if (TARGET SDL2::SDL2main)
    target_link_libraries(pi-reflection INTERFACE SDL2main)
endif ()
target_link_libraries(pi-reflection INTERFACE SDL2::SDL2 EnTT::EnTT)

#
# Install the Library
#

# install the compiled objects for pi-reflection
install(TARGETS pi-reflection
        EXPORT pi-reflection-targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/reflection)

# install the include files for pi-reflection
install(FILES
            ${CMAKE_SOURCE_DIR}/include/pi/reflection/reflect.hpp
            ${CMAKE_SOURCE_DIR}/include/pi/reflection/color.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pi/reflection)

# create and install the target files
install(EXPORT pi-reflection-targets
        FILE pi-reflection-targets.cmake
        NAMESPACE pi::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi/reflection)
