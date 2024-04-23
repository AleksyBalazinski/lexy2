#pragma once
#include <memory>
#include "array_node.hpp"
#include "function_node.hpp"
#include "leaf_node.hpp"

namespace lexy2::types {
class CloningVisitor : public TypeVisitor {
  std::unique_ptr<TypeNode> node;

 public:
  virtual void visit(const ArrayNode& arrayType) override {
    auto child = *arrayType.getChild();
    child->accept(*this);
    node = std::make_unique<ArrayNode>(arrayType.getDim(), std::move(node));
  }

  virtual void visit(const LeafNode& leafType) override {
    node = std::make_unique<LeafNode>(leafType.getPrimitiveType());
  }

  virtual void visit(const FunctionNode& functionNode) override {
    const auto& paramTypes = functionNode.getParamTypes();
    std::vector<std::unique_ptr<TypeNode>> paramNodes;
    for (const auto& paramType : paramTypes) {
      paramType->accept(*this);
      paramNodes.push_back(std::move(node));
    }
    TypeNode* returnType = functionNode.getReturnType();
    returnType->accept(*this);
    node =
        std::make_unique<FunctionNode>(std::move(paramNodes), std::move(node));
  }

  std::unique_ptr<TypeNode> getClone() { return std::move(node); }
};
}  // namespace lexy2::types
