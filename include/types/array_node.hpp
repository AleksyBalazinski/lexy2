#pragma once
#include <memory>
#include "type_node.hpp"
#include "type_visitor.hpp"

namespace lexy2::types {
class ArrayNode : public TypeNode {
  std::unique_ptr<TypeNode> child;
  int dim;

 public:
  virtual void accept(TypeVisitor& v) override {
    child->accept(v);
    v.visit(*this);
  }

  virtual bool isLeaf() const override { return false; }
  virtual std::optional<int> getSimpleTypeId() const override { return {}; }
  ArrayNode(int dim, std::unique_ptr<TypeNode> child)
      : dim(dim), child(std::move(child)) {}
  TypeNode& getChild() const { return *child; }
  int getDim() const { return dim; }
};
}  // namespace lexy2::types
