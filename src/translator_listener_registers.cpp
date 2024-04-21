#include "translator_listener.hpp"

namespace lexy2 {
Value TranslatorListener::addRegisters(const Value& left, const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("add registers");

  int typeID = *left.type.getSimpleTypeId();
  auto regStr = generator.createBinOp(
      LLVMGenerator::BinOpName::ADD,
      toLLVMType(static_cast<PrimitiveType>(typeID)), left.name, right.name);
  return Value(regStr, left.type);
}

Value TranslatorListener::subtractRegisters(const Value& left,
                                            const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("subtract registers");

  int typeID = *left.type.getSimpleTypeId();
  auto regStr = generator.createBinOp(
      LLVMGenerator::BinOpName::SUB,
      toLLVMType(static_cast<PrimitiveType>(typeID)), left.name, right.name);
  return Value(regStr, left.type);
}

Value TranslatorListener::multiplyRegisters(const Value& left,
                                            const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("nultiply registers");

  int typeID = *left.type.getSimpleTypeId();
  auto regStr = generator.createBinOp(
      LLVMGenerator::BinOpName::MUL,
      toLLVMType(static_cast<PrimitiveType>(typeID)), left.name, right.name);
  return Value(regStr, left.type);
}
Value TranslatorListener::divideRegisters(const Value& left,
                                          const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("divide registers");

  int typeID = *left.type.getSimpleTypeId();
  auto regStr = generator.createBinOp(
      LLVMGenerator::BinOpName::DIV,
      toLLVMType(static_cast<PrimitiveType>(typeID)), left.name, right.name);
  return Value(regStr, left.type);
}

Value TranslatorListener::modRegisters(const Value& left, const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("mod registers");

  int typeID = *left.type.getSimpleTypeId();
  auto regStr = generator.createBinOp(
      LLVMGenerator::BinOpName::REM,
      toLLVMType(static_cast<PrimitiveType>(typeID)), left.name, right.name);
  return Value(regStr, left.type);
}

Value TranslatorListener::castRegister(const Value& value,
                                       const types::Type& targetType) {
  if (!value.type.getSimpleTypeId().has_value() ||
      !targetType.getSimpleTypeId())
    throw std::invalid_argument("cast registers");

  int typeID = *value.type.getSimpleTypeId();
  int targetTypeID = *targetType.getSimpleTypeId();
  if (typeID == targetTypeID) {
    return value;  // do nothing
  }

  PrimitiveType targetPrimitive = static_cast<PrimitiveType>(targetTypeID);
  if (value.category == Value::Category::CONSTANT) {
    auto llvmFrom = toLLVMType(static_cast<PrimitiveType>(typeID));
    auto llvmTo = toLLVMType(targetPrimitive);
    if (generator.supportsLiteralTranslation(llvmFrom, llvmTo)) {
      auto str = generator.getLiteral(llvmFrom, llvmTo, value.name);
      return Value(str, targetType, Value::Category::CONSTANT);
    }
  }

  if (typeID == INT_TYPE_ID) {
    if (targetTypeID == DOUBLE_TYPE_ID) {
      auto regStr = generator.castI32ToDouble(value.name);
      return Value(
          regStr,
          types::Type(std::make_unique<types::LeafNode>(targetPrimitive)));
    }
  }
  if (typeID == INT_TYPE_ID) {
    if (targetTypeID == FLOAT_TYPE_ID) {
      auto regStr = generator.castI32ToFloat(value.name);
      return Value(
          regStr,
          types::Type(std::make_unique<types::LeafNode>(targetPrimitive)));
    }
  }
  if (typeID == DOUBLE_TYPE_ID) {
    if (targetTypeID == INT_TYPE_ID) {
      auto regStr = generator.castDoubleToI32(value.name);
      return Value(
          regStr,
          types::Type(std::make_unique<types::LeafNode>(targetPrimitive)));
    }
  }
  if (typeID == DOUBLE_TYPE_ID) {
    if (targetTypeID == FLOAT_TYPE_ID) {
      auto regStr = generator.truncDoubleToFloat(value.name);
      return Value(
          regStr,
          types::Type(std::make_unique<types::LeafNode>(targetPrimitive)));
    }
  }
  if (typeID == FLOAT_TYPE_ID) {
    if (targetTypeID == INT_TYPE_ID) {
      auto regStr = generator.castFloatToI32(value.name);
      return Value(
          regStr,
          types::Type(std::make_unique<types::LeafNode>(targetPrimitive)));
    }
  }
  if (typeID == FLOAT_TYPE_ID) {
    if (targetTypeID == DOUBLE_TYPE_ID) {
      auto regStr = generator.extFloatToDouble(value.name);
      return Value(
          regStr,
          types::Type(std::make_unique<types::LeafNode>(targetPrimitive)));
    }
  }
  if (typeID == BOOL_TYPE_ID) {
    if (targetTypeID == INT_TYPE_ID) {}
  }
  throw std::runtime_error("Not implemented");  // TODO: user defined types
}

Value TranslatorListener::negateRegister(const Value& value) {
  if (!value.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("negate register");

  int typeID = *value.type.getSimpleTypeId();
  auto llvmType = toLLVMType(static_cast<PrimitiveType>(typeID));
  auto regStr = generator.createBinOp(LLVMGenerator::BinOpName::SUB, llvmType,
                                      LLVMGenerator::getZeroLiteral(llvmType),
                                      value.name);
  return Value(regStr, value.type);
}

Value TranslatorListener::plusRegister(const Value& value) {
  return value;
}

Value TranslatorListener::compareRegisters(Value left, Value right,
                                           Relation rel) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("compare registers");

  int typeID = *left.type.getSimpleTypeId();
  auto regStr =
      generator.createRel(toLLVMType(static_cast<PrimitiveType>(typeID)),
                          toLLVMRel(rel), left.name, right.name);
  return Value(
      regStr,
      types::Type(std::make_unique<types::LeafNode>(PrimitiveType::BOOL)));
}

Value TranslatorListener::load(const Value& val) {
  if (!val.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("load");

  int typeID = *val.type.getSimpleTypeId();
  bool isInternalPtr = val.category == Value::Category::INTERNAL_PTR;
  if (typeID == DOUBLE_TYPE_ID) {
    return Value(
        generator.createLoad(LLVMGenerator::Type::DOUBLE, val.name,
                             isInternalPtr),
        types::Type(std::make_unique<types::LeafNode>(PrimitiveType::DOUBLE)));
  }
  if (typeID == INT_TYPE_ID) {
    return Value(
        generator.createLoad(LLVMGenerator::Type::I32, val.name, isInternalPtr),
        types::Type(std::make_unique<types::LeafNode>(PrimitiveType::INT)));
  }
  if (typeID == BOOL_TYPE_ID) {
    auto loaded =
        generator.createLoad(LLVMGenerator::Type::I8, val.name, isInternalPtr);
    return Value(
        generator.truncateI8ToI1(loaded),
        types::Type(std::make_unique<types::LeafNode>(PrimitiveType::BOOL)));
  }
  if (typeID == FLOAT_TYPE_ID) {
    return Value(
        generator.createLoad(LLVMGenerator::Type::FLOAT, val.name,
                             isInternalPtr),
        types::Type(std::make_unique<types::LeafNode>(PrimitiveType::FLOAT)));
  }
  throw std::runtime_error("Not implemented");
}

bool TranslatorListener::applyBuiltInConversions(
    Value& left, Value& right, const antlr4::ParserRuleContext* ctx) {
  bool implicitLeftRight = typeManager.isImplicitFromTo(left.type, right.type);
  bool implicitRightLeft = typeManager.isImplicitFromTo(right.type, left.type);
  if (!implicitLeftRight && !implicitRightLeft) {
    errorHandler.reportError(
        utils::getLineCol(ctx),
        "No implicit conversion exists between arguments of type ? and ?");
    return false;
  }

  if (implicitLeftRight) {
    left = castRegister(left, right.type);
  } else {
    right = castRegister(right, left.type);
  }
  return true;
}

void TranslatorListener::assignToVariable(const Value& lhs, Value& value,
                                          Lexy2Parser::AssignContext* ctx) {
  auto identifier = lhs.bareName;

  const auto [iter, depth] = symbolTable.globalFind(identifier);
  if (iter == symbolTable.end()) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Identifier '" + identifier + "' not declared");
    inErrorMode = true;
    return;
  }

  if (value.category == Value::Category::MEMORY) {
    value = load(value);
  }
  auto variable = iter->second;
  variable.name += symbolTable.getScopeID(depth);

  if (variable.type.getSimpleTypeId() != value.type.getSimpleTypeId()) {
    if (typeManager.isImplicitFromTo(value.type, variable.type)) {
      value = castRegister(value, variable.type);
    } else {
      errorHandler.reportError(utils::getLineCol(ctx),
                               "No implicit conversion from ? to ?");
      inErrorMode = true;
      return;
    }
  }

  const auto op = ctx->op->getText();
  if (op == "+=") {
    variable = load(variable);
    value = addRegisters(variable, value);
  }
  if (op == "*=") {
    variable = load(variable);
    value = multiplyRegisters(variable, value);
  }
  if (op == "/=") {
    variable = load(variable);
    value = divideRegisters(variable, value);
  }
  if (op == "%=") {
    variable = load(variable);
    value = modRegisters(variable, value);
  }
  if (op == "-=") {
    variable = load(variable);
    value = subtractRegisters(variable, value);
  }
  if (op == "=") {
    // do nothing
  }
  const auto scopedIdentifier = identifier + symbolTable.getScopeID(depth);
  if (variable.type.isLeaf()) {
    int variableTypeID = *variable.type.getSimpleTypeId();
    if (variableTypeID == DOUBLE_TYPE_ID) {
      generator.createAssignment(LLVMGenerator::Type::DOUBLE, scopedIdentifier,
                                 value.name);
    }
    if (variableTypeID == INT_TYPE_ID) {
      generator.createAssignment(LLVMGenerator::Type::I32, scopedIdentifier,
                                 value.name);
    }
    if (variableTypeID == BOOL_TYPE_ID) {
      value.name = generator.extI1toI8(value.name);
      generator.createAssignment(LLVMGenerator::Type::I8, scopedIdentifier,
                                 value.name);
    }
  }

  valueStack.push(value);
}

void TranslatorListener::assignToInternalPtr(
    const Value& lhs, Value& value,
    Lexy2Parser::AssignContext*
        ctx) {  // this is a wild code duplication but I don't care

  if (value.category == Value::Category::MEMORY) {
    value = load(value);
  }

  auto variable = lhs;
  if (variable.type.getSimpleTypeId() != value.type.getSimpleTypeId()) {
    if (typeManager.isImplicitFromTo(value.type, variable.type)) {
      value = castRegister(value, variable.type);
    } else {
      errorHandler.reportError(utils::getLineCol(ctx),
                               "No implicit conversion from ? to ?");
      inErrorMode = true;
      return;
    }
  }

  const auto op = ctx->op->getText();
  if (op == "+=") {
    variable = load(variable);
    value = addRegisters(variable, value);
  }
  if (op == "*=") {
    variable = load(variable);
    value = multiplyRegisters(variable, value);
  }
  if (op == "/=") {
    variable = load(variable);
    value = divideRegisters(variable, value);
  }
  if (op == "%=") {
    variable = load(variable);
    value = modRegisters(variable, value);
  }
  if (op == "-=") {
    variable = load(variable);
    value = subtractRegisters(variable, value);
  }
  if (op == "=") {
    // do nothing
  }
  const auto scopedIdentifier = lhs.name;
  if (variable.type.isLeaf()) {
    int variableTypeID = *variable.type.getSimpleTypeId();
    if (variableTypeID == DOUBLE_TYPE_ID) {
      generator.createAssignment(LLVMGenerator::Type::DOUBLE, scopedIdentifier,
                                 value.name, true);
    }
    if (variableTypeID == INT_TYPE_ID) {
      generator.createAssignment(LLVMGenerator::Type::I32, scopedIdentifier,
                                 value.name, true);
    }
    if (variableTypeID == BOOL_TYPE_ID) {
      value.name = generator.extI1toI8(value.name);
      generator.createAssignment(LLVMGenerator::Type::I8, scopedIdentifier,
                                 value.name, true);
    }
  }

  valueStack.push(value);
}

}  // namespace lexy2