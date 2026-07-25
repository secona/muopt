# μopt / muopt

`muopt` is a header-only C++17 micro-library for argument parsing. Its design is inspired by
[Lexopt](https://crates.io/crates/lexopt).

## Features

The following command line options are supported.

- Short arguments (`-h`)
- Long arguments (`--help`)
- Short arguments with values (`-ovalue`, `-o=value`, `-o value`)
- Chained short arguments (`-abc` results three short flags: `a`, `b`, and `c`)
- Long arguments with values (`--name=Vito`, `--name Vito`)
- Plain arguments, i.e. options without hyphen prefixes (`run`, `build`)

## Examples

```cpp
int main(int argc, char **argv) {
  muopt::Parser parser(argc, argv);

  while (auto arg = parser.next()) {
    if (arg->is_long("help"))
      std::cout << "usage: example [--input FILE]\n";
    if (arg->is_long("input"))
      std::cout << parser.arg_value().value_or("") << '\n';
  }
}
```

See the [examples](./examples) directory for more usage examples.

## Design

`muopt` acts more like a tokenizer than it is a parser; it yields Shorts, Longs, and Plains in the order they appear, with no coupling
between them.

1. The parser is created by instantiating `muopt::Parser` with `argc` and `argv`.
2. `next()` returns the next argument as a Short, Long, or Plain.
3. `next()` returns `std::nullopt` when args are exhausted.
4. `arg_value()` returns the string of the next Plain, if a Plain is next in line, consuming it and making it unavailable to the next `next()` call.
5. `arg_value()` returns `std::nullopt` otherwise. The non-Plain argument is not consumed; the next `next()` call will still see it.

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

## Motivation

In one of my Rust projects, I used Lexopt and loved it. When working on a C++ project, I tried looking for
similar argument parsers with Lexopt's simplicity and couldn't find one, so I decided to make my own. `muopt`
offers more control and is intended to be minimal, bare-bones, and without bloat.

## License

`muopt` is licensed under the MIT License. See [LICENSE](./LICENSE) for more information.

## Acknowledgements

- [lexopt](https://github.com/blyxxyz/lexopt) for the inspiration.
