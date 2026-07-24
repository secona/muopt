#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"

#define MAKE_PARSER(n, ...)                                                    \
  const char *n##_cargv[] = {"exe", __VA_ARGS__};                              \
  int n##_argc = sizeof(n##_cargv) / sizeof(const char *);                     \
  char *n##_argv[sizeof(n##_cargv) / sizeof(const char *)];                    \
  for (int i = 0; i < n##_argc; ++i)                                           \
    n##_argv[i] = const_cast<char *>(n##_cargv[i]);                            \
  muopt::Parser n(n##_argc, n##_argv)

TEST_CASE("--option") {
  MAKE_PARSER(parser, "--option");

  auto result = parser.next();
  REQUIRE(result->is_long("option"));
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("--option value") {
  MAKE_PARSER(parser, "--option", "value");

  auto arg = parser.next();
  REQUIRE(arg->is_long("option"));
  CHECK_EQ(parser.arg_value(), "value");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("--option=value") {
  MAKE_PARSER(parser, "--option=value");

  auto arg = parser.next();
  REQUIRE(arg->is_long("option"));
  CHECK_EQ(parser.arg_value(), "value");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("--option=") {
  MAKE_PARSER(parser, "--option=");

  auto arg = parser.next();
  REQUIRE(arg->is_long("option"));
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("--option --option2") {
  MAKE_PARSER(parser, "--option", "--option2");

  REQUIRE(parser.next()->is_long("option"));
  REQUIRE(parser.next()->is_long("option2"));
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("--option=value --option2") {
  MAKE_PARSER(parser, "--option=value", "--option2");

  REQUIRE(parser.next()->is_long("option"));
  REQUIRE(parser.next()->is_long("option2"));
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("--option=value=value2") {
  MAKE_PARSER(parser, "--option=value=value2");

  REQUIRE(parser.next()->is_long("option"));
  CHECK_EQ(parser.arg_value(), "value=value2");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("-o") {
  MAKE_PARSER(parser, "-o");

  REQUIRE(parser.next()->is_short('o'));
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("-o") {
  MAKE_PARSER(parser, "-o");

  REQUIRE(parser.next()->is_short('o'));
  // TODO: decide return type
  REQUIRE(parser.arg_value().empty());
}

TEST_CASE("-ovalue") {
  MAKE_PARSER(parser, "-ovalue");

  auto arg = parser.next();
  REQUIRE(arg->is_short('o'));
  CHECK_EQ(parser.arg_value(), "value");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("-o value") {
  MAKE_PARSER(parser, "-o", "value");

  auto arg = parser.next();
  REQUIRE(arg->is_short('o'));
  CHECK_EQ(parser.arg_value(), "value");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("-o=value") {
  MAKE_PARSER(parser, "-o=value");

  auto arg = parser.next();
  REQUIRE(arg->is_short('o'));
  CHECK_EQ(parser.arg_value(), "value");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("-o -p") {
  MAKE_PARSER(parser, "-o", "-p");

  REQUIRE(parser.next()->is_short('o'));
  REQUIRE(parser.next()->is_short('p'));
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("-opq") {
  MAKE_PARSER(parser, "-opq");

  REQUIRE(parser.next()->is_short('o'));
  REQUIRE(parser.next()->is_short('p'));
  REQUIRE(parser.next()->is_short('q'));
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("-opq") {
  MAKE_PARSER(parser, "-opqrst");

  REQUIRE(parser.next()->is_short('o'));
  REQUIRE(parser.next()->is_short('p'));
  CHECK_EQ(parser.arg_value(), "qrst");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("value") {
  MAKE_PARSER(parser, "value");

  CHECK_EQ(parser.next()->as_str(), "value");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("-") {
  MAKE_PARSER(parser, "-");

  CHECK_EQ(parser.next()->as_str(), "-");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("file1.txt -v file2.txt") {
  MAKE_PARSER(parser, "file1.txt", "-v", "file2.txt");

  CHECK_EQ(parser.next()->as_str(), "file1.txt");
  REQUIRE(parser.next()->is_short('v'));
  CHECK_EQ(parser.next()->as_str(), "file2.txt");
  CHECK_EQ(parser.next(), std::nullopt);
}

TEST_CASE("--") {
  MAKE_PARSER(parser, "--");

  CHECK_EQ(parser.next(), std::nullopt);
  CHECK_EQ(parser.arg_value(), "");
}

TEST_CASE("-- --help -c help") {
  MAKE_PARSER(parser, "--", "--help", "-c", "help");

  CHECK_EQ(parser.next()->as_str(), "--help");
  CHECK_EQ(parser.next()->as_str(), "-c");
  CHECK_EQ(parser.next()->as_str(), "help");
  CHECK_EQ(parser.next(), std::nullopt);
}
