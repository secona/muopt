#include "muopt/muopt.hpp"
#include <iostream>
#include <optional>
#include <string>

// Examples:
//   greeter Greetings --name Vito
//   greeter Hello --name Vito --ask "How are you?"
//   greeter "What's up" --name Vito --ask "How's Life?"

int main(int argc, char **argv) {
  muopt::Parser parser(argc, argv);

  std::optional<std::string> greeting;
  std::optional<std::string> name;
  std::optional<std::string> ask;

  while (muopt::Arg arg = parser.next()) {
    if (arg.is_plain()) {
      if (!greeting.has_value())
        greeting = arg.as_str();
    }

    if (arg.is_short('n') || arg.is_long("name")) {
      if (!name.has_value())
        name = parser.arg_value();
    }

    if (arg.is_short('a') || arg.is_long("ask")) {
      if (!ask.has_value())
        ask = parser.arg_value();
    }
  }

  std::cout << *greeting << ", " << *name << "!";
  if (ask.has_value())
    std::cout << " " << *ask;
  std::cout << '\n';

  return 0;
}
