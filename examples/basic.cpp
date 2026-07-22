// Some commands to try:
//   basic --help
//   basic --one --two --three
//   basic --one --help --two --three

#include "muopt/muopt.hpp"
#include <iostream>

int main(int argc, char **argv) {
  muopt::Parser p(argc, argv);

  while (true) {
    auto a = p.next();
    if (a.is_err())
      break;

    auto arg = a.arg();
    if (arg.is_long("help")) {
      std::cout << "μopt\n";
      break;
    }

    if (arg.is_long())
      std::cout << "Found long: " << arg.get_long() << "\n";
  }

  return 0;
}
