#pragma once
#include <optional>

template <class T>
class Error {
  std::optional<T> msg;

 public:
  Error(const T& msg) : msg(msg) {}
  Error() = default;
  bool hasError() { return msg.has_value(); }
  T message() { return *msg; }
  operator bool() { return msg.has_value(); }
};

template <class T>
Error<T> error(const T& msg) {
  return Error(msg);
}