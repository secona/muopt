#include "muopt/muopt.hpp"
#include <iostream>

// Examples:
//   verbose_counter -v
//   verbose_counter -vv
//   verbose_counter -vvvvvv
//   verbose_counter -v --verbose

int main(int argc, char **argv) {
  muopt::Parser parser(argc, argv);

  int verbosity = 0;

  while (std::optional<muopt::Arg> arg = parser.next()) {
    if (arg->is_short('v') || arg->is_long("verbose"))
      verbosity++;
  }

  std::cout << "verbosity count: " << verbosity << '\n';
}
