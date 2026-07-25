cmake_minimum_required(VERSION 3.14)
project(muopt VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# muopt Itself
# ------------------------------------------------------------------------------

add_library(muopt INTERFACE)
add_library(muopt::muopt ALIAS muopt)
target_include_directories(muopt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
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
