#include "types/leaf_node.hpp"

namespace lexy2::types {
LeafNode::LeafNode(TypeManager::TyID typeID, const TypeManager& typeManager)
    : typeID(typeID), typeManager(typeManager) {}

void LeafNode::accept(TypeVisitor& v) const {
  v.visit(*this);
}

bool LeafNode::isLeaf() const {
  return true;
}

bool LeafNode::isIntegral() const {
  return typeID == TypeManager::BOOL_TYPE_ID ||
         typeID == TypeManager::INT_TYPE_ID;
}

bool LeafNode::isFloatingPoint() const {
  return typeID == TypeManager::FLOAT_TYPE_ID ||
         typeID == TypeManager::DOUBLE_TYPE_ID;
}

bool LeafNode::isUserDefined() const {
  return !typeManager.isPrimitiveType(typeID);
}

std::optional<TypeManager::TyID> LeafNode::getSimpleTypeId() const {
  return typeID;
}

std::optional<TypeNode*> LeafNode::getChild() const {
  return {};
}

TypeManager::TyID LeafNode::getTypeID() const {
  return typeID;
}

}  // namespace lexy2::types
