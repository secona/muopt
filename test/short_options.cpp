#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"
#include "utils.hpp"

TEST_CASE("-<option>") {
  MAKE_PARSER(parser, "-o");

  REQUIRE(parser.next()->is_short('o'));
}
