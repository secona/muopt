#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"

TEST_CASE("value") {
  int argc = 2;
  char *argv[] = {(char *)"exe", (char *)"value"};
  muopt::Parser parser(argc, argv);

  CHECK_EQ(parser.next()->get_value(), "value");
}
