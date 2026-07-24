cmake_minimum_required(VERSION 3.14)
project(muopt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# muopt Itself
# ------------------------------------------------------------------------------

add_library(muopt INTERFACE)
target_include_directories(muopt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(muopt INTERFACE cxx_std_17)

# ------------------------------------------------------------------------------
# Examples
# ------------------------------------------------------------------------------

option(MUOPT_BUILD_EXAMPLES "Build example programs" ON)
if(MUOPT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

option(MUOPT_BUILD_TESTS "Build test suite" ON)
if(MUOPT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
