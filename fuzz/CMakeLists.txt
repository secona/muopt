add_executable(muopt_fuzz fuzz.cpp)
target_link_libraries(muopt_fuzz PRIVATE muopt)
target_compile_options(muopt_fuzz PRIVATE -fsanitize=fuzzer,address)
target_link_options(muopt_fuzz PRIVATE -fsanitize=fuzzer,address)
