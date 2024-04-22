#pragma once

namespace lexy2::types {
class ArrayNode;
class LeafNode;

class TypeVisitor {
 public:
  virtual void visit(const ArrayNode& type) = 0;
  virtual void visit(const LeafNode& type) = 0;
};
}  // namespace lexy2::types
