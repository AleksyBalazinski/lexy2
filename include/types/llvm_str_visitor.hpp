#pragma once
#include <string>
#include "array_node.hpp"
#include "leaf_node.hpp"

namespace lexy2::types {
class LLVMStrVisitor : public TypeVisitor {
  std::string text;

 public:
  virtual void visit(const ArrayNode& arrayType) override {
    text = "[" + std::to_string(arrayType.getDim()) + " x " + text + "]";
  }

  virtual void visit(const LeafNode& LeafNode) override {
    if (LeafNode.getPrimitiveType() == PrimitiveType::BOOL) {
      text = "i8";
      return;
    }
    text =
        LLVMGenerator::getTypeString(toLLVMType(LeafNode.getPrimitiveType()));
  }

  std::string getStr() const { return text; }

  void reset() { text = ""; }
};
}  // namespace lexy2::types
