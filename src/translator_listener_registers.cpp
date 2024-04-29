#include "translator_listener.hpp"

namespace lexy2 {
Value TranslatorListener::addRegisters(const Value& left, const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("add registers");

  auto regStr = generator.createBinOp(LLVMGenerator::BinOpName::ADD, left.type,
                                      left.name, right.name);
  return Value(regStr, left.type);
}

Value TranslatorListener::subtractRegisters(const Value& left,
                                            const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("subtract registers");

  auto regStr = generator.createBinOp(LLVMGenerator::BinOpName::SUB, left.type,
                                      left.name, right.name);
  return Value(regStr, left.type);
}

Value TranslatorListener::multiplyRegisters(const Value& left,
                                            const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("nultiply registers");

  auto regStr = generator.createBinOp(LLVMGenerator::BinOpName::MUL, left.type,
                                      left.name, right.name);
  return Value(regStr, left.type);
}
Value TranslatorListener::divideRegisters(const Value& left,
                                          const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("divide registers");

  auto regStr = generator.createBinOp(LLVMGenerator::BinOpName::DIV, left.type,
                                      left.name, right.name);
  return Value(regStr, left.type);
}

Value TranslatorListener::modRegisters(const Value& left, const Value& right) {
  if (!left.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("mod registers");

  auto regStr = generator.createBinOp(LLVMGenerator::BinOpName::REM, left.type,
                                      left.name, right.name);
  return Value(regStr, left.type);
}

Value TranslatorListener::castRegister(const Value& value,
                                       const types::Type& targetType) {

  int typeID = *value.type.getSimpleTypeId();
  int targetTypeID = *targetType.getSimpleTypeId();
  if (typeID == targetTypeID) {
    return value;  // do nothing
  }

  PrimitiveType targetPrimitive = static_cast<PrimitiveType>(targetTypeID);
  if (value.category == Value::Category::CONSTANT) {
    if (generator.supportsLiteralTranslation(value.type, targetType)) {
      auto str = generator.getLiteral(value.type, targetType, value.name);
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

  auto regStr = generator.createBinOp(LLVMGenerator::BinOpName::SUB, value.type,
                                      LLVMGenerator::getZeroLiteral(value.type),
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

  auto regStr =
      generator.createRel(left.type, toLLVMRel(rel), left.name, right.name);
  return Value(
      regStr,
      types::Type(std::make_unique<types::LeafNode>(PrimitiveType::BOOL)));
}

Value TranslatorListener::load(const Value& val) {
  if (!val.type.getSimpleTypeId().has_value())
    throw std::invalid_argument("load");

  int typeID = *val.type.getSimpleTypeId();
  bool isInternalPtr = val.category == Value::Category::INTERNAL_PTR;
  if (typeID == BOOL_TYPE_ID) {
    auto loaded =
        generator.createLoad(val.type, val.name, false, isInternalPtr);
    return Value(generator.truncateI8ToI1(loaded), types::Type(val.type));
  }
  return Value(generator.createLoad(val.type, val.name, false, isInternalPtr),
               val.type);
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
    if (variableTypeID == BOOL_TYPE_ID) {
      value.name = generator.extI1toI8(value.name);
    }
    generator.createAssignment(variable.type, scopedIdentifier, value.name,
                               false);
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
    if (variableTypeID == BOOL_TYPE_ID) {
      value.name = generator.extI1toI8(value.name);
    }
    generator.createAssignment(variable.type, scopedIdentifier, value.name,
                               false, true);
  }

  valueStack.push(value);
}

}  // namespace lexy2