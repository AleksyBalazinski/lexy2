#pragma once
#include <memory>
#include "array_node.hpp"
#include "leaf_node.hpp"

namespace lexy2::types {
class CloningVisitor : public TypeVisitor {
  std::unique_ptr<TypeNode> node;

 public:
  virtual void visit(const ArrayNode& arrayType) override {
    node = std::make_unique<ArrayNode>(arrayType.getDim(), std::move(node));
  }

  virtual void visit(const LeafNode& leafType) override {
    node = std::make_unique<LeafNode>(leafType.getPrimitiveType());
  }

  std::unique_ptr<TypeNode> getClone() { return std::move(node); }
};
}  // namespace lexy2::types
