#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"
#include "utils.hpp"

TEST_CASE("-o") {
  MAKE_PARSER(parser, "-o");

  REQUIRE(parser.next()->is_short('o'));
}

TEST_CASE("-o value") {
  MAKE_PARSER(parser, "-o", "value");

  auto arg = parser.next();
  REQUIRE(arg->is_short('o'));
  CHECK_EQ(arg->get_value(), "value");
}

TEST_CASE("-o -p") {
  MAKE_PARSER(parser, "-o", "-p");

  REQUIRE(parser.next()->is_short('o'));
  REQUIRE(parser.next()->is_short('p'));
}
