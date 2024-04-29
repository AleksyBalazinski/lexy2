#include "types/llvm_str_visitor.hpp"

namespace lexy2::types {
LLVMStrVisitor::LLVMStrVisitor(bool boolAsI1) : boolAsI1(boolAsI1) {}
void types::LLVMStrVisitor::visit(const ArrayNode& arrayType) {
  auto child = *arrayType.getChild();
  child->accept(*this);
  text = "[" + std::to_string(arrayType.getDim()) + " x " + text + "]";
}

std::string getTypeString(PrimitiveType primitiveType, bool boolAsI1) {
  switch (primitiveType) {
    case PrimitiveType::INT:
      return "i32";
    case PrimitiveType::DOUBLE:
      return "double";
    case PrimitiveType::BOOL:
      return boolAsI1 ? "i1" : "i8";
    case PrimitiveType::FLOAT:
      return "float";
    default:
      return "";
  }
}

void types::LLVMStrVisitor::visit(const LeafNode& LeafNode) {
  text = getTypeString(LeafNode.getPrimitiveType(), boolAsI1);
}

void types::LLVMStrVisitor::visit(const FunctionNode& functionNode) {
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

std::string types::LLVMStrVisitor::getStr() const {
  return text;
}

void types::LLVMStrVisitor::reset() {
  text = "";
}
}  // namespace lexy2::types
