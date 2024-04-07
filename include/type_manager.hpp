#pragma once

#include <array>
#include <optional>
#include <stdexcept>
#include <utility>
#include "llvm_generator.hpp"

namespace lexy2 {

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
enum class PrimitiveType { INT, DOUBLE, BOOL };

LLVMGenerator::Type toLLVMType(PrimitiveType primitiveType);

class TypeManager {
  bool implicitConversions[3][3] = {
      //      to
      // ---------------
      // int double bool
      {false, true, false},    // int    |
      {false, false, false},   // double |  from
      {false, false, false}};  // bool   |

  bool operatorSupport[13][3] = {
      // int double bool
      {true, true, false},   // add
      {true, true, false},   // sub
      {true, true, false},   // mul
      {true, true, false},   // div
      {true, false, false},  // rem
      {true, true, false},   // neg
      {true, true, true},    // pos
      {true, true, false},   // lt
      {true, true, false},   // gt
      {true, true, false},   // le
      {true, true, false},   // ge
      {true, true, false},   // eq
      {true, true, false}    // ne
  };

 public:
  bool isImplicitFromTo(int from, int to) {
    return implicitConversions[from][to];
  }

  bool isOperatorSupported(Operator op, int type) {
    return operatorSupport[static_cast<int>(op)][type];
  }
};  // namespace lexy2
}  // namespace lexy2