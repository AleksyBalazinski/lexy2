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
  FunctionNode(std::vector<std::unique_ptr<TypeNode>> paramTypes,
               std::unique_ptr<TypeNode> returnType);

  virtual void accept(TypeVisitor& v) const override;

  virtual bool isLeaf() const override;

  virtual bool isIntegral() const override;

  virtual bool isFloatingPoint() const override;

  virtual std::optional<int> getSimpleTypeId() const override;

  virtual std::optional<TypeNode*> getChild() const override;

  const std::vector<std::unique_ptr<TypeNode>>& getParamTypes() const;

  TypeNode* getReturnType() const;
};
}  // namespace lexy2::types