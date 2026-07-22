cmake_minimum_required(VERSION 3.14)
project(uopt LANGUAGES CXX)

# ------------------------------------------------------------------------------
# uopt Itself
# ------------------------------------------------------------------------------

add_library(uopt INTERFACE)
target_include_directories(uopt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(uopt INTERFACE cxx_std_17)

# ------------------------------------------------------------------------------
# Examples
# ------------------------------------------------------------------------------

option(UOPT_BUILD_EXAMPLES "Build example programs" ON)
if(UOPT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

option(UOPT_BUILD_TESTS "Build test suite" ON)
if(UOPT_BUILD_TESTS)
    add_subdirectory(test)
endif()
