#pragma once
#include <memory>
#include <vector>
#include "type_node.hpp"
#include "type_visitor.hpp"

namespace lexy2::types {
class FunctionNode : public TypeNode {
  std::vector<std::unique_ptr<TypeNode>> paramTypes;
  std::unique_ptr<TypeNode> returnType;

 public:
  virtual void accept(TypeVisitor& v) const override { v.visit(*this); }

  virtual bool isLeaf() const override { return false; }

  virtual std::optional<int> getSimpleTypeId() const override { return {}; }

  FunctionNode(std::vector<std::unique_ptr<TypeNode>> paramTypes,
               std::unique_ptr<TypeNode> returnType)
      : paramTypes(std::move(paramTypes)), returnType(std::move(returnType)) {}

  virtual std::optional<TypeNode*> getChild() const override { return {}; }

  const std::vector<std::unique_ptr<TypeNode>>& getParamTypes() const {
    return paramTypes;
  }

  TypeNode* getReturnType() const { return returnType.get(); }
};
}  // namespace lexy2::types