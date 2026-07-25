#include "muopt/muopt.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  std::vector<std::string> storage;
  std::vector<const char *> args;

  storage.reserve(size + 1);
  args.reserve(size + 2);
  args.push_back("fuzz");

  size_t start = 0;
  for (size_t i = 0; i <= size; ++i) {
    if (i == size || data[i] == '\0') {
      size_t len = i - start;
      if (len > 0) {
        storage.emplace_back(reinterpret_cast<const char *>(data + start), len);
        args.push_back(storage.back().c_str());
      }
      start = i + 1;
    }
  }

  int argc = static_cast<int>(args.size());
  char **argv = const_cast<char **>(args.data());
  muopt::Parser parser(argc, argv);

  while (auto arg = parser.next()) {
    if (arg->is_short()) {
      (void)arg->as_char();
      (void)parser.arg_value();
    } else if (arg->is_long()) {
      (void)arg->as_str();
      (void)parser.arg_value();
    } else if (arg->is_plain()) {
      (void)arg->as_str();
    }
  }

  return 0;
}
