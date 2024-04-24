#pragma once
#include <memory>
#include <optional>
#include "cloning_visitor.hpp"
#include "type_node.hpp"
#include "type_visitor.hpp"

namespace lexy2::types {
class Type {
  std::unique_ptr<TypeNode> root;

 public:
  Type() : Type(nullptr) {}
  Type(std::unique_ptr<TypeNode> root) : root(std::move(root)) {}

  Type(const Type& other) {
    CloningVisitor cloningVisitor;
    other.applyVisitor(cloningVisitor);
    root = cloningVisitor.getClone();
  }

  Type& operator=(const Type& other) {
    CloningVisitor cloningVisitor;
    other.applyVisitor(cloningVisitor);
    root = cloningVisitor.getClone();
    return *this;
  }

  void applyVisitor(TypeVisitor& v) const { root->accept(v); }

  bool isLeaf() const { return root->isLeaf(); }

  std::optional<int> getSimpleTypeId() const { return root->getSimpleTypeId(); }

  std::optional<Type> getPeeledType() const {
    auto child = root->getChild();
    if (!child.has_value()) {
      return {};
    }

    CloningVisitor cloningVisitor;
    (*child)->accept(cloningVisitor);
    return cloningVisitor.getClone();
  }

  const TypeNode& getRoot() const { return *root; }
};
}  // namespace lexy2::types
