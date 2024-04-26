#include "types/array_node.hpp"

namespace lexy2::types {
ArrayNode::ArrayNode(int dim, std::unique_ptr<TypeNode> child)
    : dim(dim), child(std::move(child)) {}

void ArrayNode::accept(TypeVisitor& v) const {
  v.visit(*this);
}

bool ArrayNode::isLeaf() const {
  return false;
}

std::optional<int> ArrayNode::getSimpleTypeId() const {
  return {};
}

std::optional<TypeNode*> ArrayNode::getChild() const {
  return child.get();
}

int ArrayNode::getDim() const {
  return dim;
}

}  // namespace lexy2::types
