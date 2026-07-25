include(FetchContent)
find_package(doctest QUIET)

if(NOT doctest_FOUND)
    message(STATUS "doctest not found. fetching from GitHub")
    FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.5.3
    )
    FetchContent_MakeAvailable(doctest)
endif()

# ------------------------------------------------------------------------------
# muopt main tests
# ------------------------------------------------------------------------------

add_executable(muopt_test muopt_test.cpp)
target_link_libraries(muopt_test PRIVATE muopt doctest::doctest)
add_test(NAME muopt_test COMMAND muopt_test)

if(MSVC)
    target_compile_options(muopt_test PRIVATE /utf-8)
endif()

if(WIN32)
    add_executable(muopt_wide_to_utf8_test wide_to_utf8_test.cpp)
    target_link_libraries(muopt_wide_to_utf8_test PRIVATE muopt doctest::doctest)
    if(MSVC)
        target_compile_options(muopt_wide_to_utf8_test PRIVATE /utf-8)
    endif()
    add_test(NAME muopt_wide_to_utf8_test COMMAND muopt_wide_to_utf8_test)
endif()
