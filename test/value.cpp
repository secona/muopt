#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"
#include "utils.hpp"

TEST_CASE("value") {
  MAKE_PARSER(parser, "value");

  CHECK_EQ(parser.next()->get_value(), "value");
}
