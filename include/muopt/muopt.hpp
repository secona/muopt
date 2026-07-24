#ifndef MUOPT_MUOPT_HPP_
#define MUOPT_MUOPT_HPP_

#include <cassert>
#include <optional>
#include <string_view>
#include <variant>

namespace muopt {

class Arg {
public:
  enum class Kind {
    Short,
    Long,
    Value,
  };

  static Arg make_short(char c) { return Arg(Kind::Short, c, {}, {}); }
  static Arg make_long(std::string_view n) {
    return Arg(Kind::Long, {}, n, {});
  }
  static Arg make_value(std::string_view v) {
    return Arg(Kind::Value, {}, {}, v);
  }

  bool is_short() { return kind_ == Kind::Short; }
  bool is_short(char c) { return is_short() && short_ == c; }
  bool is_long() { return kind_ == Kind::Long; }
  bool is_long(std::string_view n) { return is_long() && long_ == n; }
  bool is_value() { return kind_ == Kind::Value; }

  Kind get_kind() { return kind_; }
  char get_short() {
    assert(is_short());
    return short_;
  }
  std::string_view get_long() {
    assert(is_long());
    return long_;
  }
  std::string_view get_value() {
    assert(is_value());
    return value_;
  }

private:
  Kind kind_;
  char short_;
  std::string_view long_;
  std::string_view value_;

  Arg(Kind k, char c, std::string_view n, std::string_view v)
      : kind_(k), short_(c), long_(n), value_(v) {}
};

class Parser {
public:
  Parser(int argc, char **argv) : argc_(argc), argv_(argv), index_(1) {}

  std::optional<Arg> next() {
    if (index_ >= argc_)
      return std::nullopt;

    std::string_view arg = argv_[index_++];

    // match `--<option>`
    if (arg.rfind("--", 0) == 0 && arg.length() > 2) {
      std::string_view opt = arg.substr(2);
      return Arg::make_long(opt);
    }

    // match `-<option>`
    if (arg.front() == '-') {
      std::string_view opt = arg.substr(1, 2);
      return Arg::make_short(opt.front());
    }

    return std::nullopt;
  }

private:
  int argc_;
  char **argv_;
  int index_;
};

} // namespace muopt

#endif // MUOPT_MUOPT_HPP_
