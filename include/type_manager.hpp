#pragma once
#include <array>
#include <optional>
#include <stdexcept>
#include <utility>
#include "llvm_generator.hpp"

namespace lexy2 {

enum class Operator { ADD, SUB, MUL, DIV, REM };
enum class PrimitiveType { INT, DOUBLE, BOOL };

inline LLVMGenerator::Type toLLVMType(PrimitiveType primitiveType) {
  switch (primitiveType) {
    case PrimitiveType::INT:
      return LLVMGenerator::Type::I32;
    case PrimitiveType::DOUBLE:
      return LLVMGenerator::Type::DOUBLE;
    case PrimitiveType::BOOL:
      return LLVMGenerator::Type::I8;  // TODO this should be i1
    default:
      throw std::runtime_error("Not implemented");
  }
}

class TypeManager {
  bool implicitConversions[3][3] = {
      //      to
      // ---------------
      // int double bool
      {false, true, false},    // int    |
      {false, false, false},   // double |  from
      {false, false, false}};  // bool   |

  bool operatorSupport[5][3] = {
      // int double bool
      {true, true, false},    // add
      {true, true, false},    // sub
      {true, true, false},    // mul
      {true, true, false},    // div
      {true, false, false}};  // rem

 public:
  bool isImplicitFromTo(int from, int to) {
    return implicitConversions[from][to];
  }

  bool isOperatorSupported(Operator op, int type) {
    return operatorSupport[static_cast<int>(op)][type];
  }
};  // namespace lexy2
}  // namespace lexy2