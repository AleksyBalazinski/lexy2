#include "types/reference_node.hpp"

namespace lexy2::types {
ReferenceNode::ReferenceNode(std::unique_ptr<TypeNode> child)
    : child(std::move(child)) {}

void ReferenceNode::accept(TypeVisitor& v) const {
  v.visit(*this);
}

bool ReferenceNode::isLeaf() const {
  return false;
}

bool ReferenceNode::isIntegral() const {
  return false;
}

bool ReferenceNode::isFloatingPoint() const {
  return false;
}

std::optional<int> ReferenceNode::getSimpleTypeId() const {
  return child->getSimpleTypeId();
}

std::optional<TypeNode*> ReferenceNode::getChild() const {
  return child.get();
}
}  // namespace lexy2::types
