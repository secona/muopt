#ifndef MUOPT_MUOPT_HPP_
#define MUOPT_MUOPT_HPP_

#include <cassert>
#include <cstdint>
#include <optional>
#include <string_view>

namespace muopt {

class Arg {
public:
  enum class Kind {
    Short = 1 << 0,
    Long = 1 << 1,
    Value = 1 << 2,
  };

  friend constexpr Kind operator|(Kind a, Kind b) {
    return static_cast<Kind>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
  }

  friend constexpr Kind operator&(Kind a, Kind b) {
    return static_cast<Kind>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
  }

  static Arg make_short(char c) { return Arg(Kind::Short, c, {}, {}); }
  static Arg make_long(std::string_view n) {
    return Arg(Kind::Long, {}, n, {});
  }
  static Arg make_value(std::string_view v) {
    return Arg(Kind::Value, {}, {}, v);
  }

  bool is_kind(Kind kind) { return (kind_ & kind) == kind; }
  bool is_short() { return is_kind(Kind::Short); }
  bool is_long() { return is_kind(Kind::Long); }
  bool is_value() { return is_kind(Kind::Value); }

  bool is_short(char c) { return is_short() && short_ == c; }
  bool is_long(std::string_view n) { return is_long() && long_ == n; }

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

  friend class Parser;

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
      std::optional<Arg> val = next();

      auto arg = Arg::make_long(opt);
      if (val.has_value() && val->is_value()) {
        arg.value_ = val->get_value();
        arg.kind_ = arg.kind_ | Arg::Kind::Value;
      }
      return arg;
    }

    // match `-<option>`
    if (arg.front() == '-') {
      std::string_view opt = arg.substr(1, 2);
      std::optional<Arg> val = next();

      auto arg = Arg::make_short(opt.front());
      if (val.has_value() && val->is_value()) {
        arg.value_ = val->get_value();
        arg.kind_ = arg.kind_ | Arg::Kind::Value;
      }
      return arg;
    }

    // match `<value>`
    std::string_view opt = arg;
    return Arg::make_value(opt);
  }

private:
  int argc_;
  char **argv_;
  int index_;
};

} // namespace muopt

#endif // MUOPT_MUOPT_HPP_
