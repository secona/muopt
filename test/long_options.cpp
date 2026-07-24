#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"
#include "utils.hpp"

TEST_CASE("--option") {
  MAKE_PARSER(parser, "--option");

  auto result = parser.next();
  REQUIRE(result->is_long("option"));
}

TEST_CASE("--option value") {
  MAKE_PARSER(parser, "--option", "value");

  auto arg = parser.next();
  REQUIRE(arg->is_long("option"));
  CHECK_EQ(parser.get_value(), "value");
}

TEST_CASE("--option=value") {
  MAKE_PARSER(parser, "--option=value");

  auto arg = parser.next();
  REQUIRE(arg->is_long("option"));
  CHECK_EQ(parser.get_value(), "value");
}

TEST_CASE("--option --option2") {
  MAKE_PARSER(parser, "--option", "--option2");

  REQUIRE(parser.next()->is_long("option"));
  REQUIRE(parser.next()->is_long("option2"));
}
