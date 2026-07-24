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

#define MU_LONG(n) CHECK(parser.next()->is_long(n))
#define MU_SHORT(n) CHECK(parser.next()->is_short(n))
#define MU_PLAIN(n) CHECK(parser.next()->is_plain(n))

#define MU_LONG_WITH_VALUE(n, v)                                               \
  MU_LONG(n);                                                                  \
  CHECK_EQ(parser.arg_value(), v)

#define MU_SHORT_WITH_VALUE(n, v)                                              \
  MU_SHORT(n);                                                                 \
  CHECK_EQ(parser.arg_value(), v)

#define MU_LONG_NO_VALUE(n) MU_LONG_WITH_VALUE(n, std::nullopt)
#define MU_SHORT_NO_VALUE(n) MU_SHORT_WITH_VALUE(n, std::nullopt)

#define MU_END() CHECK_EQ(parser.next(), std::nullopt)

TEST_CASE("--option") {
  MAKE_PARSER(parser, "--option");

  MU_LONG_NO_VALUE("option");
  MU_END();
}

TEST_CASE("--option value") {
  MAKE_PARSER(parser, "--option", "value");

  MU_LONG_WITH_VALUE("option", "value");
  MU_END();
}

TEST_CASE("--option=value") {
  MAKE_PARSER(parser, "--option=value");

  MU_LONG_WITH_VALUE("option", "value");
  MU_END();
}

TEST_CASE("--option=") {
  MAKE_PARSER(parser, "--option=");

  MU_LONG_WITH_VALUE("option", "");
  MU_END();
}

TEST_CASE("--option --option2") {
  MAKE_PARSER(parser, "--option", "--option2");

  MU_LONG_NO_VALUE("option");
  MU_LONG_NO_VALUE("option2");
  MU_END();
}

TEST_CASE("--option=value --option2") {
  MAKE_PARSER(parser, "--option=value", "--option2");

  MU_LONG_WITH_VALUE("option", "value");
  MU_LONG_NO_VALUE("option2");
  MU_END();
}

TEST_CASE("--option=value=value2") {
  MAKE_PARSER(parser, "--option=value=value2");

  MU_LONG_WITH_VALUE("option", "value=value2");
  MU_END();
}

TEST_CASE("-o") {
  MAKE_PARSER(parser, "-o");

  MU_SHORT_NO_VALUE('o');
  MU_END();
}

TEST_CASE("-ovalue") {
  MAKE_PARSER(parser, "-ovalue");

  MU_SHORT_WITH_VALUE('o', "value");
  MU_END();
}

TEST_CASE("-o value") {
  MAKE_PARSER(parser, "-o", "value");

  MU_SHORT_WITH_VALUE('o', "value");
  MU_END();
}

TEST_CASE("-o=value") {
  MAKE_PARSER(parser, "-o=value");

  MU_SHORT_WITH_VALUE('o', "value");
  MU_END();
}

TEST_CASE("-o -p") {
  MAKE_PARSER(parser, "-o", "-p");

  MU_SHORT_NO_VALUE('o');
  MU_SHORT_NO_VALUE('p');
  MU_END();
}

TEST_CASE("-opq") {
  MAKE_PARSER(parser, "-opq");

  MU_SHORT('o');
  MU_SHORT('p');
  MU_SHORT('q');
  MU_END();
}

TEST_CASE("-opqrst") {
  MAKE_PARSER(parser, "-opqrst");

  MU_SHORT('o');
  MU_SHORT('p');
  MU_SHORT_WITH_VALUE('q', "rst");
  MU_END();
}

TEST_CASE("value") {
  MAKE_PARSER(parser, "value");

  MU_PLAIN("value");
  MU_END();
}

TEST_CASE("-") {
  MAKE_PARSER(parser, "-");

  MU_PLAIN("-");
  MU_END();
}

TEST_CASE("file1.txt -v file2.txt") {
  MAKE_PARSER(parser, "file1.txt", "-v", "file2.txt");

  MU_PLAIN("file1.txt");
  MU_SHORT('v');
  MU_PLAIN("file2.txt");
  MU_END();
}

TEST_CASE("--") {
  MAKE_PARSER(parser, "--");

  MU_END();
}

TEST_CASE("-- --help -c help") {
  MAKE_PARSER(parser, "--", "--help", "-c", "help");

  MU_PLAIN("--help");
  MU_PLAIN("-c");
  MU_PLAIN("help");
  MU_END();
}

TEST_CASE("-o=") {
  MAKE_PARSER(parser, "-o=");

  MU_SHORT_WITH_VALUE('o', "");
  MU_END();
}
