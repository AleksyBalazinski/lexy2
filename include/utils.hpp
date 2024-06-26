#pragma once

#include <stack>
#include <utility>
#include "antlr4-runtime.h"

namespace lexy2::utils {
inline std::pair<int, int> getLineCol(const antlr4::ParserRuleContext* ctx) {
  auto tok = ctx->getStart();
  return std::make_pair<int, int>(tok->getLine(), tok->getCharPositionInLine());
}

template <class T>
inline std::pair<T, T> popTwo(std::stack<T>& stack) {
  const auto right = stack.top();
  stack.pop();
  const auto left = stack.top();
  stack.pop();
  return std::make_pair(left, right);
}

template <class T>
inline std::pair<T, T> peekTwo(std::stack<T>& stack) {
  const auto right = stack.top();
  stack.pop();
  const auto left = stack.top();
  stack.push(right);
  return std::make_pair(left, right);
}
}  // namespace lexy2::utils