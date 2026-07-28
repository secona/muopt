cmake_minimum_required(VERSION 3.14)
project(muopt VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# muopt Itself
# ------------------------------------------------------------------------------

add_library(muopt INTERFACE)
add_library(muopt::muopt ALIAS muopt)
target_include_directories(
    muopt INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(muopt INTERFACE cxx_std_17)

# ------------------------------------------------------------------------------
# Options
# ------------------------------------------------------------------------------

set(MUOPT_STANDALONE_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MUOPT_STANDALONE_PROJECT ON)
endif()

option(MUOPT_BUILD_EXAMPLES "Build example programs" ${MUOPT_STANDALONE_PROJECT})
option(MUOPT_BUILD_TESTS "Build test suite" ${MUOPT_STANDALONE_PROJECT})
option(MUOPT_BUILD_FUZZ "Build fuzz targets" OFF)

# ------------------------------------------------------------------------------
# Examples
# ------------------------------------------------------------------------------

if(MUOPT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

if(MUOPT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# ------------------------------------------------------------------------------
# Fuzz targets
# ------------------------------------------------------------------------------

if(MUOPT_BUILD_FUZZ)
    add_subdirectory(fuzz)
endif()

# ------------------------------------------------------------------------------
# Install & Package Export
# ------------------------------------------------------------------------------

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/muoptConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/muoptConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/muopt
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/muoptConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(DIRECTORY include/muopt DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS muopt EXPORT muoptTargets)

install(
    EXPORT muoptTargets
    FILE muoptTargets.cmake
    NAMESPACE muopt::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/muopt
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/muoptConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/muoptConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/muopt
)
