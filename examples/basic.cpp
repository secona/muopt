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
      std::cout << "Found long: " << arg->get_long() << "\n";
    if (arg->is_short())
      std::cout << "Found short: " << arg->get_short() << "\n";
  }

  return 0;
}
