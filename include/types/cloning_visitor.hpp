#pragma once
#include <memory>
#include "array_node.hpp"
#include "function_node.hpp"
#include "leaf_node.hpp"
#include "reference_node.hpp"

namespace lexy2::types {
class CloningVisitor : public TypeVisitor {
  std::unique_ptr<TypeNode> node;

 public:
  virtual void visit(const ArrayNode& arrayType) override;

  virtual void visit(const LeafNode& leafType) override;

  virtual void visit(const FunctionNode& functionNode) override;

  virtual void visit(const ReferenceNode& referenceNode) override;

  std::unique_ptr<TypeNode> getClone();
};
}  // namespace lexy2::types
