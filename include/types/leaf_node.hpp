#pragma once

#include "primitive_type.hpp"
#include "type_node.hpp"
#include "type_visitor.hpp"

namespace lexy2::types {
class LeafNode : public TypeNode {
  PrimitiveType primitiveType;

 public:
  LeafNode(PrimitiveType primitiveType);

  virtual void accept(TypeVisitor& v) const override;

  virtual bool isLeaf() const override;

  virtual bool isIntegral() const override;

  virtual bool isFloatingPoint() const override;

  virtual std::optional<int> getSimpleTypeId() const override;

  virtual std::optional<TypeNode*> getChild() const override;

  PrimitiveType getPrimitiveType() const;
};
}  // namespace lexy2::types
