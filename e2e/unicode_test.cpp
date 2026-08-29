#include "muopt/muopt.hpp"
#include <cassert>

int main(int argc, char **argv) {
  muopt::Parser parser(argc, argv);

  auto arg1 = parser.next();
  assert(arg1);
  assert(arg1.is_long("name"));
  assert(parser.arg_value() == "café");

  auto arg2 = parser.next();
  assert(arg2);
  assert(arg2.is_long("file"));
  assert(parser.arg_value() == "bücher.txt");

  auto arg3 = parser.next();
  assert(arg3);
  assert(arg3.is_plain("résumé.txt"));

  assert(!parser.next());
  return 0;
}
