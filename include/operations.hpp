#pragma once

#include <stdexcept>

namespace lexy2 {
enum class Relation { EQ, NEQ, LT, GT, GE, LE };

enum class Operator {
  ADD,
  SUB,
  MUL,
  DIV,
  REM,
  NEG,
  POS,
  LT,
  GT,
  LE,
  GE,
  EQ,
  NE
};
}  // namespace lexy2
