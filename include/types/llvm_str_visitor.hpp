#pragma once
#include <stdexcept>
#include <string>
#include "array_node.hpp"
#include "function_node.hpp"
#include "leaf_node.hpp"
#include "reference_node.hpp"

namespace lexy2::types {
class LLVMStrVisitor : public TypeVisitor {
  std::string text;
  bool boolAsI1;

 public:
  LLVMStrVisitor(bool boolAsI1 = false);

  virtual void visit(const ArrayNode& arrayType) override;

  virtual void visit(const LeafNode& LeafNode) override;

  virtual void visit(const FunctionNode& functionNode) override;

  virtual void visit(const ReferenceNode& referenceNode) override;

  std::string getStr() const;

  void reset();
};
}  // namespace lexy2::types
