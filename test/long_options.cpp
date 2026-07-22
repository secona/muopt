#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "uopt/uopt.hpp"

TEST_CASE("--<option>") {
  int argc = 2;
  char *argv[] = {(char *)"exe", (char *)"--option"};
  uopt::Parser parser(argc, argv);

  auto result = parser.next();

  REQUIRE(result.arg().is_long("option"));
}
