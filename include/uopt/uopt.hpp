#ifndef UOPT_UOPT_HPP_
#define UOPT_UOPT_HPP_

#include <cassert>
#include <string_view>
#include <variant>

namespace uopt {

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

class Err {};

class Result {
public:
  Result(Arg arg) : v_(arg) {}
  Result(Err err) : v_(err) {}

  bool is_arg() const { return std::holds_alternative<Arg>(v_); }
  bool is_err() const { return std::holds_alternative<Err>(v_); }

  Arg arg() const { return std::get<Arg>(v_); }
  Err err() const { return std::get<Err>(v_); }

private:
  std::variant<Arg, Err> v_;
};

class Parser {
public:
  Parser(int argc, char **argv) : argc_(argc), argv_(argv), index_(1) {}

  Result next() {
    if (index_ > argc_)
      return Result(Err{});

    std::string_view arg = argv_[index_++];

    // match `--<option>`
    if (arg.rfind("--", 0) == 0 && arg.length() > 2) {
      std::string_view opt = arg.substr(2);
      return Arg::make_long(opt);
    }

    return Result(Err{});
  }

private:
  int argc_;
  char **argv_;
  int index_;
};

} // namespace uopt

#endif // UOPT_UOPT_HPP_
