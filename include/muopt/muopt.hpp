#ifndef MUOPT_MUOPT_HPP_
#define MUOPT_MUOPT_HPP_

#include <cassert>
#include <optional>
#include <string_view>
#include <utility>

namespace muopt {

class Arg {
public:
  enum class Kind {
    None,
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

  explicit operator bool() const { return kind_ != Kind::None; }

  bool is_short() const { return kind_ == Kind::Short; }
  bool is_long() const { return kind_ == Kind::Long; }
  bool is_plain() const { return kind_ == Kind::Plain; }

  bool is_short(char c) const { return is_short() && short_ == c; }
  bool is_long(std::string_view n) const { return is_long() && str_ == n; }
  bool is_plain(std::string_view n) const { return is_plain() && str_ == n; }

  char as_char() const {
    assert(is_short());
    return short_;
  }
  std::string_view as_str() const {
    assert(is_long() || is_plain());
    return str_;
  }

  friend class Parser;

private:
  Kind kind_;
  char short_;
  std::string_view str_;

  Arg() : kind_(Kind::None), short_('\0'), str_() {}
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

private:
  std::optional<Arg> next_impl() {
    if (buffer_.has_value())
      return std::exchange(buffer_, std::nullopt);

    if (state_ == State::Shorts) {
      if (pending_val_.has_value() && !pending_val_->empty()) {
        auto arg = Arg::make_short(pending_val_->front());
        pending_val_->remove_prefix(1);
        if (pending_val_->empty())
          pending_val_ = std::nullopt;
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
    pending_val_ = std::nullopt;

    if (arg_str == "--") {
      state_ = State::DoubleDashed;
      return next_impl();
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
        if (pending_val_->front() == '=')
          pending_val_->remove_prefix(1);
        else
          state_ = State::Shorts;
      }

      return Arg::make_short(raw.front());
    }

    // match `<value>`
    return Arg::make_plain(arg_str);
  }

public:
  Arg next() {
    auto maybe_arg = next_impl();
    return maybe_arg.has_value() ? std::move(*maybe_arg) : Arg();
  }

  std::optional<std::string_view> arg_value() {
    if (pending_val_.has_value()) {
      auto res = *pending_val_;
      pending_val_ = std::nullopt;
      return res;
    }

    auto arg = next();
    if (!arg)
      return {}; // TODO: maybe return error?
    if (arg.is_plain())
      return arg.as_str();

    buffer_ = std::move(arg);
    return {};
  }

private:
  int argc_;
  char **argv_;
  int index_;

  State state_;

  std::optional<std::string_view> pending_val_;
  std::optional<Arg> buffer_;
};

} // namespace muopt

#endif // MUOPT_MUOPT_HPP_
