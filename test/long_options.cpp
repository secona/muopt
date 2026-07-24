#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"

TEST_CASE("--<option>") {
  int argc = 2;
  char *argv[] = {(char *)"exe", (char *)"--option"};
  muopt::Parser parser(argc, argv);

  auto result = parser.next();

  REQUIRE(result->is_long("option"));
}
