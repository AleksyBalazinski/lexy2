#include "type_manager.hpp"

namespace lexy2 {
const TypeManager::TyID TypeManager::INT_TYPE_ID;
const TypeManager::TyID TypeManager::DOUBLE_TYPE_ID;
const TypeManager::TyID TypeManager::BOOL_TYPE_ID;
const TypeManager::TyID TypeManager::FLOAT_TYPE_ID;

TypeManager::TypeManager() {
  typeIDs["int"] = INT_TYPE_ID;
  typeIDs["double"] = DOUBLE_TYPE_ID;
  typeIDs["bool"] = BOOL_TYPE_ID;
  typeIDs["float"] = FLOAT_TYPE_ID;

  primitiveTypeIDs.insert(INT_TYPE_ID);
  primitiveTypeIDs.insert(DOUBLE_TYPE_ID);
  primitiveTypeIDs.insert(BOOL_TYPE_ID);
  primitiveTypeIDs.insert(FLOAT_TYPE_ID);
}

bool TypeManager::isImplicitFromTo(const types::Type& from,
                                   const types::Type& to) const {
  if (!from.isLeaf() || !to.isLeaf()) {
    return false;
  }
  int fromID = *from.getSimpleTypeId();
  int toID = *to.getSimpleTypeId();
  return implicitConversions[fromID][toID];
}

bool TypeManager::isOperatorSupported(Operator op,
                                      const types::Type& type) const {
  if (!type.isLeaf()) {
    return false;
  }
  int typeID = *type.getSimpleTypeId();
  return operatorSupport[static_cast<int>(op)][typeID];
}

bool TypeManager::addType(const std::string& typeName) {
  if (typeIDs.find(typeName) != typeIDs.end()) {
    return false;
  }

  typeIDs[typeName] = typeIDs.size();
  return true;
}

bool TypeManager::addStruct(TyID typeID, Struct&& struct_) {
  if (structs.find(typeID) != structs.end()) {
    return false;
  }

  structs.insert(std::make_pair(typeID, std::move(struct_)));
  return true;
}

bool TypeManager::isStruct(TyID typeID) {
  return structs.find(typeID) != structs.end();
}

const Struct& TypeManager::getStruct(TyID typeID) const {
  return structs.at(typeID);
}

std::optional<TypeManager::TyID> TypeManager::getTypeID(
    const std::string& typeName) const {
  if (typeIDs.find(typeName) == typeIDs.end()) {
    return {};
  }

  return typeIDs.at(typeName);
}

bool TypeManager::isPrimitiveType(TyID typeID) const {
  return primitiveTypeIDs.find(typeID) != primitiveTypeIDs.end();
}

}  // namespace lexy2
