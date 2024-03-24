cmake_minimum_required(VERSION 3.18)
project(pi)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# most modules will use these libraries
find_package(SDL2 REQUIRED)
find_package(EnTT REQUIRED)

# define the modules in subdirectories
add_subdirectory(src/events)

# generate version for config file
set(version 0.1.0)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pi-config-version.cmake"
        VERSION "${version}"
        COMPATIBILITY AnyNewerVersion)

# create config file
configure_package_config_file(
        pi-config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/pi-config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)

#install config files
install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/pi-config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/pi-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pi)