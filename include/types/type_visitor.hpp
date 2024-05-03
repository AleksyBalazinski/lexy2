#pragma once

namespace lexy2::types {
class ArrayNode;
class LeafNode;
class FunctionNode;
class ReferenceNode;

class TypeVisitor {
 public:
  virtual void visit(const ArrayNode& type) = 0;
  virtual void visit(const LeafNode& type) = 0;
  virtual void visit(const FunctionNode& type) = 0;
  virtual void visit(const ReferenceNode& type) = 0;
};
}  // namespace lexy2::types
