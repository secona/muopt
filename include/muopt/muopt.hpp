#ifndef MUOPT_MUOPT_HPP_
#define MUOPT_MUOPT_HPP_

#include <cassert>
#include <cstdint>
#include <optional>
#include <string_view>
#include <utility>

namespace muopt {

class Arg {
public:
  enum class Kind {
    Short,
    Long,
    Plain,
  };

  static Arg make_short(char c) {
    Arg a;
    a.kind_ = Kind::Short;
    a.short_ = c;
    return a;
  }
  static Arg make_long(std::string_view n) {
    Arg a;
    a.kind_ = Kind::Long;
    a.str_ = n;
    return a;
  }
  static Arg make_plain(std::string_view v) {
    Arg a;
    a.kind_ = Kind::Plain;
    a.str_ = v;
    return a;
  }

  bool is_short() { return kind_ == Kind::Short; }
  bool is_long() { return kind_ == Kind::Long; }
  bool is_plain() { return kind_ == Kind::Plain; }

  bool is_short(char c) { return is_short() && short_ == c; }
  bool is_long(std::string_view n) { return is_long() && str_ == n; }

  char as_char() {
    assert(is_short());
    return short_;
  }
  std::string_view as_str() {
    assert(is_long() || is_plain());
    return str_;
  }

  friend class Parser;

private:
  Kind kind_;
  char short_;
  std::string_view str_;

  Arg() = default;
};

class Parser {
private:
  enum class State {
    None,
    Shorts,
    DoubleDashed,
  };

public:
  Parser(int argc, char **argv)
      : argc_(argc), argv_(argv), index_(1), state_(State::None) {}

  std::optional<Arg> next() {
    if (buffer_.has_value())
      return std::exchange(buffer_, std::nullopt);

    if (state_ == State::Shorts) {
      if (!pending_val_.empty()) {
        auto arg = Arg::make_short(pending_val_.front());
        pending_val_.remove_prefix(1);
        return arg;
      }

      state_ = State::None;
    }

    if (index_ >= argc_)
      return std::nullopt;

    std::string_view arg_str = argv_[index_++];

    if (state_ == State::DoubleDashed)
      return Arg::make_plain(arg_str);

    // flush pendingval
    pending_val_ = {};

    if (arg_str == "--") {
      state_ = State::DoubleDashed;
      return next();
    }

    // match `--<option>`
    if (arg_str.rfind("--", 0) == 0 && arg_str.length() > 2) {
      std::string_view raw = arg_str.substr(2);

      size_t eq = raw.find('=');
      if (eq != std::string_view::npos) {
        pending_val_ = raw.substr(eq + 1);
        return Arg::make_long(raw.substr(0, eq));
      }

      return Arg::make_long(raw);
    }

    // match `-<option>`
    if (arg_str.front() == '-') {
      std::string_view raw = arg_str.substr(1);

      // argument is only `-`
      if (raw.empty())
        return Arg::make_plain(arg_str);

      if (raw.length() > 1) {
        pending_val_ = raw.substr(1);
        if (pending_val_.front() == '=')
          pending_val_.remove_prefix(1);
        state_ = State::Shorts;
      }

      return Arg::make_short(raw.front());
    }

    // match `<value>`
    return Arg::make_plain(arg_str);
  }

  std::string_view arg_value() {
    if (!pending_val_.empty()) {
      std::string_view res = pending_val_;
      pending_val_ = {};
      return res;
    }

    auto maybe_value = next();
    if (!maybe_value.has_value())
      return {}; // TODO: maybe return error?
    if (maybe_value->is_plain())
      return maybe_value->as_str();

    buffer_ = std::move(maybe_value);
    return {};
  }

private:
  int argc_;
  char **argv_;
  int index_;

  State state_;

  std::string_view pending_val_;
  std::optional<Arg> buffer_;
};

} // namespace muopt

#endif // MUOPT_MUOPT_HPP_
