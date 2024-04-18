#pragma once

#include "primitive_type.hpp"
#include "type_node.hpp"
#include "type_visitor.hpp"

namespace lexy2::types {
class LeafNode : public TypeNode {
  PrimitiveType primitiveType;

 public:
  LeafNode(PrimitiveType primitiveType) : primitiveType(primitiveType) {}
  virtual void accept(TypeVisitor& v) override { v.visit(*this); }
  virtual bool isLeaf() const override { return true; }
  virtual std::optional<int> getSimpleTypeId() const override {
    return static_cast<int>(primitiveType);
  }
  PrimitiveType getPrimitiveType() const { return primitiveType; }
};
}  // namespace lexy2::types
