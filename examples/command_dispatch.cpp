#include "muopt/muopt.hpp"
#include <iostream>
#include <optional>
#include <string>

// Examples:
//   command_dispatch
//   command_dispatch build
//   command_dispatch build --release
//   command_dispatch run
//   command_dispatch run --port=9000
//   command_dispatch test
//   command_dispatch test unittests

int main(int argc, char **argv) {
  muopt::Parser parser(argc, argv);

  std::optional<std::string> command;

  std::optional<muopt::Arg> arg = parser.next();
  if (arg.has_value() && arg->is_plain()) {
    command = arg->as_str();
  }

  if (!command.has_value()) {
    std::cerr << "usage: command_dispatch <command> [options]\n";
    return 1;
  }

  if (*command == "build") {
    bool release = false;
    while (std::optional<muopt::Arg> arg = parser.next()) {
      if (arg->is_long("release"))
        release = true;
    }

    std::cout << "building (" << (release ? "release" : "debug") << ")\n";
    return 0;
  }

  if (*command == "run") {
    std::optional<std::string> port;
    while (std::optional<muopt::Arg> arg = parser.next()) {
      if (arg->is_long("port"))
        port = parser.arg_value();
    }

    std::cout << "running on port " << port.value_or("3000") << "\n";
    return 0;
  }

  if (*command == "test") {
    std::optional<muopt::Arg> arg = parser.next();
    std::optional<std::string> directory;
    if (arg.has_value() && arg->is_plain())
      directory = arg->as_str();

    std::cout << "running tests";
    if (directory.has_value())
      std::cout << " in " << *directory;
    std::cout << "\n";
    return 0;
  }

  std::cerr << "unknown command: " << *command << '\n';
  return 1;
}
