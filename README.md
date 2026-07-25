# μopt / muopt

`muopt` is a header-only C++17 micro-library for argument parsing. Its design is inspired by
[Lexopt](https://crates.io/crates/lexopt).

## Integration

<details>
  <summary>CMake (FetchContent)</summary>
  
  ```cmake
  include(FetchContent)
  FetchContent_Declare(
      muopt
      GIT_REPOSITORY https://github.com/secona/muopt.git
      GIT_TAG main
  )
  FetchContent_MakeAvailable(muopt)
  target_link_libraries(program PRIVATE muopt::muopt)
  ```
</details>

<details>
  <summary>CMake (vendored)</summary>

  ```cmake
  add_subdirectory(vendor/muopt)
  target_link_libraries(program PRIVATE muopt::muopt)
  ```
</details>

<details>
  <summary>Bazel (Bzlmod)</summary>

  ```bazel
  # in MODULE.bazel

  bazel_dep(name = "muopt")
  
  git_override(
      module_name = "muopt",
      commit = "bedd3f76720c9790b0fd3175f8e28eaab3c3c13e", # change as needed
      remote = "https://github.com/secona/muopt.git",
  )
  ```
</details>

<details>
  <summary>Bazel</summary>

  ```bazel
  new_git_repository(
      name = "muopt",
      commit = "bedd3f76720c9790b0fd3175f8e28eaab3c3c13e", # change as needed
      init_submodules = False,
      remote = "https://github.com/secona/muopt.git",
  )
  ```
</details>

## Examples

See the [examples](./examples) directory for usage examples.

## Motivation

In one of my Rust projects, I used Lexopt and loved it. When working on a C++ project, I tried looking for
similar argument parsers with Lexopt's simplicity and couldn't find one, so I decided to make my own. `muopt`
offers more control and is intended to be minimal, bare-bones, and without bloat.

## License

`muopt` is licensed under the MIT License. See [LICENSE](./LICENSE) for more information.
