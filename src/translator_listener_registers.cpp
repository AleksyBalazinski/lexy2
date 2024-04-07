#include "translator_listener.hpp"

namespace lexy2 {
Value TranslatorListener::addRegisters(const Value& left, const Value& right) {
  auto regStr =
      generator.createBinOp(LLVMGenerator::BinOpName::ADD,
                            toLLVMType(static_cast<PrimitiveType>(left.typeID)),
                            left.name, right.name);
  return Value(regStr, left.typeID);
}

Value TranslatorListener::subtractRegisters(const Value& left,
                                            const Value& right) {
  auto regStr =
      generator.createBinOp(LLVMGenerator::BinOpName::SUB,
                            toLLVMType(static_cast<PrimitiveType>(left.typeID)),
                            left.name, right.name);
  return Value(regStr, left.typeID);
}

Value TranslatorListener::multiplyRegisters(const Value& left,
                                            const Value& right) {
  auto regStr =
      generator.createBinOp(LLVMGenerator::BinOpName::MUL,
                            toLLVMType(static_cast<PrimitiveType>(left.typeID)),
                            left.name, right.name);
  return Value(regStr, left.typeID);
}
Value TranslatorListener::divideRegisters(const Value& left,
                                          const Value& right) {
  auto regStr =
      generator.createBinOp(LLVMGenerator::BinOpName::DIV,
                            toLLVMType(static_cast<PrimitiveType>(left.typeID)),
                            left.name, right.name);
  return Value(regStr, left.typeID);
}

Value TranslatorListener::modRegisters(const Value& left, const Value& right) {
  auto regStr =
      generator.createBinOp(LLVMGenerator::BinOpName::REM,
                            toLLVMType(static_cast<PrimitiveType>(left.typeID)),
                            left.name, right.name);
  return Value(regStr, left.typeID);
}

Value TranslatorListener::castRegister(const Value& value, int targetType) {
  if (value.typeID == targetType) {
    return value;  // do nothing
  }
  if (value.typeID == INT_TYPE_ID) {
    if (targetType == DOUBLE_TYPE_ID) {
      auto regStr = generator.castI32ToDouble(value.name);
      return Value(regStr, DOUBLE_TYPE_ID);
    }
  }
  if (value.typeID == DOUBLE_TYPE_ID) {
    if (targetType == INT_TYPE_ID) {
      auto regStr = generator.castDoubleToI32(value.name);
      return Value(regStr, INT_TYPE_ID);
    }
  }
  if (value.typeID == BOOL_TYPE_ID) {
    if (targetType == INT_TYPE_ID) {}
  }
  throw std::runtime_error("Not implemented");  // TODO: user defined types
}

Value TranslatorListener::negateRegister(const Value& value) {
  auto llvmType = toLLVMType(static_cast<PrimitiveType>(value.typeID));
  auto regStr = generator.createBinOp(LLVMGenerator::BinOpName::SUB, llvmType,
                                      LLVMGenerator::getZeroLiteral(llvmType),
                                      value.name);
  return Value(regStr, value.typeID);
}

Value TranslatorListener::plusRegister(const Value& value) {
  return value;
}

Value TranslatorListener::compareRegisters(Value left, Value right,
                                           Relation rel) {
  auto regStr =
      generator.createRel(toLLVMType(static_cast<PrimitiveType>(left.typeID)),
                          toLLVMRel(rel), left.name, right.name);
  return Value(regStr, BOOL_TYPE_ID);
}

Value TranslatorListener::load(const Value& val) {
  if (val.typeID == DOUBLE_TYPE_ID) {
    return Value(generator.createLoad(LLVMGenerator::Type::DOUBLE, val.name),
                 DOUBLE_TYPE_ID);
  }
  if (val.typeID == INT_TYPE_ID) {
    return Value(generator.createLoad(LLVMGenerator::Type::I32, val.name),
                 INT_TYPE_ID);
  }
  if (val.typeID == BOOL_TYPE_ID) {
    auto loaded = generator.createLoad(LLVMGenerator::Type::I8, val.name);
    return Value(generator.truncateI8ToI1(loaded), BOOL_TYPE_ID);
  }
  throw std::runtime_error("Not implemented");
}

bool TranslatorListener::applyBuiltInConversions(
    Value& left, Value& right, const antlr4::ParserRuleContext* ctx) {
  bool implicitLeftRight =
      typeManager.isImplicitFromTo(left.typeID, right.typeID);
  bool implicitRightLeft =
      typeManager.isImplicitFromTo(right.typeID, left.typeID);
  if (!implicitLeftRight && !implicitRightLeft) {
    errorHandler.reportError(
        utils::getLineCol(ctx),
        "No implicit conversion exists between arguments of type ? and ?");
    return false;
  }

  if (implicitLeftRight) {
    left = castRegister(left, right.typeID);
  } else {
    right = castRegister(right, left.typeID);
  }
  return true;
}
}  // namespace lexy2