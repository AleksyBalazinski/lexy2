#include "types/cloning_visitor.hpp"

namespace lexy2::types {
void CloningVisitor::visit(const ArrayNode& arrayType) {
  auto child = *arrayType.getChild();
  child->accept(*this);
  node = std::make_unique<ArrayNode>(arrayType.getDim(), std::move(node));
}

void CloningVisitor::visit(const LeafNode& leafType) {
  node = std::make_unique<LeafNode>(leafType.getTypeID(), leafType.typeManager);
}

void CloningVisitor::visit(const FunctionNode& functionNode) {
  const auto& paramTypes = functionNode.getParamTypes();
  std::vector<std::unique_ptr<TypeNode>> paramNodes;
  for (const auto& paramType : paramTypes) {
    paramType->accept(*this);
    paramNodes.push_back(std::move(node));
  }
  TypeNode* returnType = functionNode.getReturnType();
  returnType->accept(*this);
  node = std::make_unique<FunctionNode>(std::move(paramNodes), std::move(node));
}

void CloningVisitor::visit(const ReferenceNode& referenceNode) {
  auto child = *referenceNode.getChild();
  child->accept(*this);
  node = std::make_unique<ReferenceNode>(std::move(node));
}

std::unique_ptr<TypeNode> CloningVisitor::getClone() {
  return std::move(node);
}

}  // namespace lexy2::types
