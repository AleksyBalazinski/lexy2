#pragma once

#include "llvm_generator.hpp"
#include "operations.hpp"
#include "primitive_type.hpp"
#include "types/type.hpp"

namespace lexy2 {

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
  bool isImplicitFromTo(const types::Type& from, const types::Type& to) {
    if (!from.isLeaf() || !to.isLeaf()) {
      return false;
    }
    int fromID = *from.getSimpleTypeId();
    int toID = *to.getSimpleTypeId();
    return implicitConversions[fromID][toID];
  }

  bool isOperatorSupported(Operator op, const types::Type& type) {
    if (!type.isLeaf()) {
      return false;
    }
    int typeID = *type.getSimpleTypeId();
    return operatorSupport[static_cast<int>(op)][typeID];
  }
};  // namespace lexy2
}  // namespace lexy2