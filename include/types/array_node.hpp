#pragma once
#include <memory>
#include "type_node.hpp"
#include "type_visitor.hpp"

namespace lexy2::types {
class ArrayNode : public TypeNode {
  std::unique_ptr<TypeNode> child;
  int dim;

 public:
  ArrayNode(int dim, std::unique_ptr<TypeNode> child);

  virtual void accept(TypeVisitor& v) const override;

  virtual bool isLeaf() const override;

  virtual bool isIntegral() const override;

  virtual bool isFloatingPoint() const override;

  virtual std::optional<int> getSimpleTypeId() const override;

  virtual std::optional<TypeNode*> getChild() const override;

  int getDim() const;
};
}  // namespace lexy2::types
