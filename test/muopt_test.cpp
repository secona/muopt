#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"
#include <vector>

// -----------------------------------------------------------------------------
// Harness
// -----------------------------------------------------------------------------

template <typename Func>
void muopt_execute_test(Func f, std::initializer_list<const char *> args) {
  std::vector<char *> argv = {const_cast<char *>("exe")};
  argv.reserve(args.size() + 1);
  for (auto &s : args)
    argv.push_back(const_cast<char *>(s));

  muopt::Parser parser(argv.size(), argv.data(), false);
  f(parser);
}

#define MU_TEST_IMPL(name, ...)                                                \
  static void name(muopt::Parser &);                                           \
  TEST_CASE(#__VA_ARGS__) { muopt_execute_test(name, {__VA_ARGS__}); }         \
  static void name(muopt::Parser &parser)

#define MU_TEST(...)                                                           \
  MU_TEST_IMPL(DOCTEST_ANONYMOUS(DOCTEST_MU_IMPL_), __VA_ARGS__)

// -----------------------------------------------------------------------------
// Asserts
// -----------------------------------------------------------------------------

#define MU_LONG(n) CHECK(parser.next().is_long(n))
#define MU_SHORT(n) CHECK(parser.next().is_short(n))
#define MU_PLAIN(n) CHECK(parser.next().is_plain(n))

#define MU_LONG_WITH_VALUE(n, v)                                               \
  MU_LONG(n);                                                                  \
  CHECK_EQ(parser.arg_value(), v)

#define MU_SHORT_WITH_VALUE(n, v)                                              \
  MU_SHORT(n);                                                                 \
  CHECK_EQ(parser.arg_value(), v)

#define MU_LONG_NO_VALUE(n) MU_LONG_WITH_VALUE(n, std::nullopt)
#define MU_SHORT_NO_VALUE(n) MU_SHORT_WITH_VALUE(n, std::nullopt)

#define MU_END() CHECK_FALSE(parser.next())

// -----------------------------------------------------------------------------
// Test cases
// -----------------------------------------------------------------------------

MU_TEST("--option") {
  MU_LONG_NO_VALUE("option");
  MU_END();
}

MU_TEST("--option", "value") {
  MU_LONG_WITH_VALUE("option", "value");
  MU_END();
}

MU_TEST("--option=value") {
  MU_LONG_WITH_VALUE("option", "value");
  MU_END();
}

MU_TEST("--option=") {
  MU_LONG_WITH_VALUE("option", "");
  MU_END();
}

MU_TEST("--option", "--option2") {
  MU_LONG_NO_VALUE("option");
  MU_LONG_NO_VALUE("option2");
  MU_END();
}

MU_TEST("--option=value", "--option2") {
  MU_LONG_WITH_VALUE("option", "value");
  MU_LONG_NO_VALUE("option2");
  MU_END();
}

MU_TEST("--option=value=value2") {
  MU_LONG_WITH_VALUE("option", "value=value2");
  MU_END();
}

MU_TEST("-o") {
  MU_SHORT_NO_VALUE('o');
  MU_END();
}

MU_TEST("-ovalue") {
  MU_SHORT_WITH_VALUE('o', "value");
  MU_END();
}

MU_TEST("-o", "value") {
  MU_SHORT_WITH_VALUE('o', "value");
  MU_END();
}

MU_TEST("-o=value") {
  MU_SHORT_WITH_VALUE('o', "value");
  MU_END();
}

MU_TEST("-o", "-p") {
  MU_SHORT_NO_VALUE('o');
  MU_SHORT_NO_VALUE('p');
  MU_END();
}

MU_TEST("-opq") {
  MU_SHORT('o');
  MU_SHORT('p');
  MU_SHORT('q');
  MU_END();
}

MU_TEST("-opqrst") {
  MU_SHORT('o');
  MU_SHORT('p');
  MU_SHORT_WITH_VALUE('q', "rst");
  MU_END();
}

MU_TEST("value") {
  MU_PLAIN("value");
  MU_END();
}

MU_TEST("-") {
  MU_PLAIN("-");
  MU_END();
}

MU_TEST("file1.txt", "-v", "file2.txt") {
  MU_PLAIN("file1.txt");
  MU_SHORT('v');
  MU_PLAIN("file2.txt");
  MU_END();
}

MU_TEST("--") { MU_END(); }

MU_TEST("--", "--help", "-c", "help") {
  MU_PLAIN("--help");
  MU_PLAIN("-c");
  MU_PLAIN("help");
  MU_END();
}

MU_TEST("-o=") {
  MU_SHORT_WITH_VALUE('o', "");
  MU_END();
}

MU_TEST("--café:re") {
  MU_LONG_NO_VALUE("café:re");
  MU_END();
}

MU_TEST("--name=Übel") {
  MU_LONG_WITH_VALUE("name", "Übel");
  MU_END();
}

MU_TEST("--テスト") {
  MU_LONG_NO_VALUE("テスト");
  MU_END();
}

MU_TEST("こんにちは") {
  MU_PLAIN("こんにちは");
  MU_END();
}

MU_TEST("--π=3.14") {
  MU_LONG_WITH_VALUE("π", "3.14");
  MU_END();
}
