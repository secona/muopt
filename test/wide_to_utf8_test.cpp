#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "muopt/muopt.hpp"

TEST_CASE("wide_to_utf8 conversion") {
  CHECK_EQ(muopt::detail::wide_to_utf8(L""), "");
  CHECK_EQ(muopt::detail::wide_to_utf8(L"hello"), "hello");
  CHECK_EQ(muopt::detail::wide_to_utf8(L"café:re"), "café:re");
  CHECK_EQ(muopt::detail::wide_to_utf8(L"--name=café:re"), "--name=café:re");
  CHECK_EQ(muopt::detail::wide_to_utf8(L"こんにちは"), "こんにちは");
  CHECK_EQ(muopt::detail::wide_to_utf8(L"😀"), "😀");
}
