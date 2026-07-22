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

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

enable_testing()
foreach(src ${TEST_SOURCES})
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE uopt doctest::doctest)
    add_test(NAME ${name} COMMAND ${name})
endforeach()
