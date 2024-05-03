#include "types/llvm_str_visitor.hpp"

namespace lexy2::types {
LLVMStrVisitor::LLVMStrVisitor(bool boolAsI1) : boolAsI1(boolAsI1) {}

void types::LLVMStrVisitor::visit(const ArrayNode& arrayType) {
  auto child = *arrayType.getChild();
  child->accept(*this);
  text = "[" + std::to_string(arrayType.getDim()) + " x " + text + "]";
}

std::string getTypeString(TypeManager::TyID typeID, bool boolAsI1) {
  switch (typeID) {
    case TypeManager::INT_TYPE_ID:
      return "i32";
    case TypeManager::DOUBLE_TYPE_ID:
      return "double";
    case TypeManager::BOOL_TYPE_ID:
      return boolAsI1 ? "i1" : "i8";
    case TypeManager::FLOAT_TYPE_ID:
      return "float";
    default:
      return "";
  }
}

void types::LLVMStrVisitor::visit(const LeafNode& LeafNode) {
  if (LeafNode.isUserDefined()) {
    auto typeID = LeafNode.getTypeID();
    const auto& struct_ = LeafNode.typeManager.getStruct(typeID);
    text = "%struct." + struct_.name;
    return;
  }
  text = getTypeString(LeafNode.getTypeID(), boolAsI1);
}

void types::LLVMStrVisitor::visit(const FunctionNode& functionNode) {
  // TODO this doesn't return anything useful, should be moved to some debugging visitor
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

void types::LLVMStrVisitor::visit(const ReferenceNode& referenceNode) {
  auto child = *referenceNode.getChild();
  child->accept(*this);
  text = text + "*";
}

std::string types::LLVMStrVisitor::getStr() const {
  return text;
}

void types::LLVMStrVisitor::reset() {
  text = "";
}
}  // namespace lexy2::types
