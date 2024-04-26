#include "types/type.hpp"

namespace lexy2::types {
Type::Type() : Type(nullptr) {}

Type::Type(std::unique_ptr<TypeNode> root) : root(std::move(root)) {}

Type::Type(const Type& other) {
  CloningVisitor cloningVisitor;
  other.applyVisitor(cloningVisitor);
  root = cloningVisitor.getClone();
}

Type& Type::operator=(const Type& other) {
  CloningVisitor cloningVisitor;
  other.applyVisitor(cloningVisitor);
  root = cloningVisitor.getClone();
  return *this;
}

Type::Type(Type&& other) : root(std::move(other.root)) {}

void Type::applyVisitor(TypeVisitor& v) const {
  root->accept(v);
}

bool Type::isLeaf() const {
  return root->isLeaf();
}

std::optional<int> Type::getSimpleTypeId() const {
  return root->getSimpleTypeId();
}

std::optional<Type> Type::getPeeledType() const {
  auto child = root->getChild();
  if (!child.has_value()) {
    return {};
  }

  CloningVisitor cloningVisitor;
  (*child)->accept(cloningVisitor);
  return cloningVisitor.getClone();
}

std::unique_ptr<TypeNode> cloneNode(const TypeNode& node) {
  CloningVisitor cv;
  node.accept(cv);
  return cv.getClone();
}
}  // namespace lexy2::types
