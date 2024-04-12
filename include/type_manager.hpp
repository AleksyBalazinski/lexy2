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
enum class PrimitiveType { INT, DOUBLE, BOOL, FLOAT };

LLVMGenerator::Type toLLVMType(PrimitiveType primitiveType);

class TypeManager {
  bool implicitConversions[4][4] = {
      //      to
      // ---------------
      // int double bool float
      {false, true, false, true},    // int    |
      {false, false, false, false},  // double |  from
      {false, false, false, false},  // bool   |
      {false, true, false, false}};  // float  |

  bool operatorSupport[13][4] = {
      // int double bool float
      {true, true, false, true},    // add
      {true, true, false, true},    // sub
      {true, true, false, true},    // mul
      {true, true, false, true},    // div
      {true, false, false, false},  // rem
      {true, true, false, true},    // neg
      {true, true, true, true},     // pos
      {true, true, false, true},    // lt
      {true, true, false, true},    // gt
      {true, true, false, true},    // le
      {true, true, false, true},    // ge
      {true, true, false, true},    // eq
      {true, true, false, true}     // ne
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