// Some commands to try:
//   basic --help
//   basic --one --two --three
//   basic --one --help --two --three

#include "muopt/muopt.hpp"
#include <iostream>

int main(int argc, char **argv) {
  muopt::Parser p(argc, argv);

  while (auto arg = p.next()) {
    if (arg->is_long("help")) {
      std::cout << "μopt\n";
      break;
    }

    if (arg->is_long())
      std::cout << "Found long: " << arg->as_str() << "\n";
    if (arg->is_short())
      std::cout << "Found short: " << arg->as_char() << "\n";
    if (arg->is_plain())
      std::cout << "Found value: " << arg->as_str() << "\n";
  }

  return 0;
}
