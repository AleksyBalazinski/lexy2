#pragma once

#include <unordered_map>
#include <unordered_set>
#include "operations.hpp"
#include "types/type.hpp"

namespace lexy2 {

class TypeManager {
 public:
  using TyID = int;

  TypeManager();

  bool isImplicitFromTo(const types::Type& from, const types::Type& to) const;

  bool isOperatorSupported(Operator op, const types::Type& type) const;

  bool addType(const std::string& typeName);

  std::optional<TyID> getTypeID(const std::string& typeName) const;

  bool isPrimitiveType(TyID typeID) const;

  static const TyID INT_TYPE_ID = 0;
  static const TyID DOUBLE_TYPE_ID = 1;
  static const TyID BOOL_TYPE_ID = 2;
  static const TyID FLOAT_TYPE_ID = 3;

 private:
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

  std::unordered_map<std::string, TyID> typeIDs;

  std::unordered_set<TyID> primitiveTypeIDs;

};  // namespace lexy2
}  // namespace lexy2