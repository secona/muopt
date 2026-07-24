#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"

TEST_CASE("-<option>") {
  int argc = 2;
  char *argv[] = {(char *)"exe", (char *)"-o"};
  muopt::Parser parser(argc, argv);

  REQUIRE(parser.next()->is_short('o'));
}
