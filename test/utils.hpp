#pragma once
#include "muopt/muopt.hpp"

#define MAKE_PARSER(n, ...)                                                    \
  const char *n##_cargv[] = {"exe", __VA_ARGS__};                              \
  int n##_argc = sizeof(n##_cargv) / sizeof(const char *);                     \
  char *n##_argv[sizeof(n##_cargv) / sizeof(const char *)];                    \
  for (int i = 0; i < n##_argc; ++i)                                           \
    n##_argv[i] = const_cast<char *>(n##_cargv[i]);                            \
  muopt::Parser n(n##_argc, n##_argv)
