#pragma once
#include <stdexcept>
#include <string>
#include "array_node.hpp"
#include "function_node.hpp"
#include "leaf_node.hpp"

namespace lexy2::types {
class LLVMStrVisitor : public TypeVisitor {
  std::string text;
  bool boolAsI1;

 public:
  LLVMStrVisitor(bool boolAsI1 = false) : boolAsI1(boolAsI1) {}

  virtual void visit(const ArrayNode& arrayType) override {
    auto child = *arrayType.getChild();
    child->accept(*this);
    text = "[" + std::to_string(arrayType.getDim()) + " x " + text + "]";
  }

  virtual void visit(const LeafNode& LeafNode) override {
    if (LeafNode.getPrimitiveType() == PrimitiveType::BOOL) {
      if (boolAsI1) {
        text = "i1";
        return;
      }
      text = "i8";
      return;
    }
    text =
        LLVMGenerator::getTypeString(toLLVMType(LeafNode.getPrimitiveType()));
  }

  virtual void visit(const FunctionNode& functionNode) override {
    // TODO this doesn't return nothing useful, should be moved to some debugging visitor
    //throw std::runtime_error("Not implemented");
    std::string functionTypeStr;
    const auto& paramTypes = functionNode.getParamTypes();
    functionTypeStr += "(";
    for (const auto& paramType : paramTypes) {
      paramType->accept(*this);
      functionTypeStr += text + ", ";
    }
    functionTypeStr += ") -> ";
    TypeNode* returnType = functionNode.getReturnType();
    returnType->accept(*this);
    functionTypeStr += text;
    text = functionTypeStr;
  }

  std::string getStr() const { return text; }

  void reset() { text = ""; }
};
}  // namespace lexy2::types
