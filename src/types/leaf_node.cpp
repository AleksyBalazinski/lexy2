#include "types/leaf_node.hpp"

namespace lexy2::types {
LeafNode::LeafNode(PrimitiveType primitiveType)
    : primitiveType(primitiveType) {}

void LeafNode::accept(TypeVisitor& v) const {
  v.visit(*this);
}

bool LeafNode::isLeaf() const {
  return true;
}

std::optional<int> LeafNode::getSimpleTypeId() const {
  return static_cast<int>(primitiveType);
}

std::optional<TypeNode*> LeafNode::getChild() const {
  return {};
}

PrimitiveType LeafNode::getPrimitiveType() const {
  return primitiveType;
}

}  // namespace lexy2::types
