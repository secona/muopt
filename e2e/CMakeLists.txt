add_executable(muopt_unicode_test unicode_test.cpp)
target_link_libraries(muopt_unicode_test PRIVATE muopt)
if(MSVC)
    target_compile_options(muopt_unicode_test PRIVATE /utf-8)
endif()

add_test(
    NAME muopt_unicode_test
    COMMAND muopt_unicode_test --name=café --file=bücher.txt résumé.txt
)
