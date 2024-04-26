#include "types/function_node.hpp"

namespace lexy2::types {
FunctionNode::FunctionNode(std::vector<std::unique_ptr<TypeNode>> paramTypes,
                           std::unique_ptr<TypeNode> returnType)
    : paramTypes(std::move(paramTypes)), returnType(std::move(returnType)) {}

void FunctionNode::accept(TypeVisitor& v) const {
  v.visit(*this);
}

bool FunctionNode::isLeaf() const {
  return false;
}

std::optional<int> FunctionNode::getSimpleTypeId() const {
  return {};
}

std::optional<TypeNode*> FunctionNode::getChild() const {
  return {};
}

const std::vector<std::unique_ptr<TypeNode>>& FunctionNode::getParamTypes()
    const {
  return paramTypes;
}

TypeNode* FunctionNode::getReturnType() const {
  return returnType.get();
}

}  // namespace lexy2::types
