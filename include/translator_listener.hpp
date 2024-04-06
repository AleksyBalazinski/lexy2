#pragma once

#include <optional>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>

#include "Lexy2BaseListener.h"
#include "error_handler.hpp"
#include "llvm_generator.hpp"
#include "operations.hpp"
#include "symbol_table.hpp"
#include "type_manager.hpp"
#include "utils.hpp"
namespace lexy2 {

class TranslatorListener : public Lexy2BaseListener {
  std::stack<Value> valueStack;
  std::stack<std::string> basicBlockStack;
  std::stack<std::string> returnPointsStack;
  SymbolTable symbolTable;
  LLVMGenerator generator;
  const int INT_TYPE_ID = static_cast<int>(PrimitiveType::INT);
  const int DOUBLE_TYPE_ID = static_cast<int>(PrimitiveType::DOUBLE);
  const int BOOL_TYPE_ID = static_cast<int>(PrimitiveType::BOOL);
  std::unordered_map<std::string, int> typeIDs;
  TypeManager typeManager;

  ErrorHandler& errorHandler;
  bool inErrorMode = false;
  bool encounteredErrors = false;

 public:
  TranslatorListener(ErrorHandler& errorHandler) : errorHandler(errorHandler) {
    typeIDs.insert(std::make_pair("int", INT_TYPE_ID));
    typeIDs.insert(std::make_pair("double", DOUBLE_TYPE_ID));
    typeIDs.insert(std::make_pair("bool", BOOL_TYPE_ID));
  }

  void exitTranslationUnit(Lexy2Parser::TranslationUnitContext* ctx) override {}

  void exitStatement(Lexy2Parser::StatementContext* ctx) override;

  void exitExprStatement(Lexy2Parser::ExprStatementContext* ctx) override {}

  void exitPrintIntrinsic(Lexy2Parser::PrintIntrinsicContext* ctx) override;

  void exitDeclStatement(Lexy2Parser::DeclStatementContext* ctx) override;

  void enterCompoundStatement(
      Lexy2Parser::CompoundStatementContext* ctx) override;
  void exitCompoundStatement(
      Lexy2Parser::CompoundStatementContext* ctx) override;

  void exitCondition(Lexy2Parser::ConditionContext* ctx) override;

  void enterThenPart(Lexy2Parser::ThenPartContext* ctx) override;
  void exitThenPart(Lexy2Parser::ThenPartContext* ctx) override;

  void enterElsePart(Lexy2Parser::ElsePartContext* ctx) override;
  void exitElsePart(Lexy2Parser::ElsePartContext* ctx) override;

  void enterIf(Lexy2Parser::IfContext* ctx) override;
  void exitIf(Lexy2Parser::IfContext* ctx) override;

  void enterIfElse(Lexy2Parser::IfElseContext* ctx) override;
  void exitIfElse(Lexy2Parser::IfElseContext* ctx) override;

  void exitComma(Lexy2Parser::CommaContext* ctx) override {}

  void exitTernary(Lexy2Parser::TernaryContext* ctx) override {}

  void exitAssign(Lexy2Parser::AssignContext* ctx) override;

  void exitLogicalAnd(Lexy2Parser::LogicalAndContext* ctx) override {}

  void exitLogicalOr(Lexy2Parser::LogicalOrContext* ctx) override {}

  void exitEquality(Lexy2Parser::EqualityContext* ctx) override;

  void exitRelation(Lexy2Parser::RelationContext* ctx) override;

  void exitAdditive(Lexy2Parser::AdditiveContext* ctx) override;

  void exitMultiplicative(Lexy2Parser::MultiplicativeContext* ctx) override;

  void exitCast(Lexy2Parser::CastContext* ctx) override;

  void exitUnary(Lexy2Parser::UnaryContext* ctx) override;

  void exitIdenitifer(Lexy2Parser::IdenitiferContext* ctx) override;

  void exitParens(Lexy2Parser::ParensContext* ctx) override {}

  void exitIntegerLiteral(Lexy2Parser::IntegerLiteralContext* ctx) override;

  void exitFloatLiteral(Lexy2Parser::FloatLiteralContext* ctx) override;

  void exitBoolLiteral(Lexy2Parser::BoolLiteralContext* ctx) override;

  std::string getCode(const std::string& sourceFilename) {
    return generator.emitCode(sourceFilename);
  }

 private:
  Value addRegisters(const Value& left, const Value& right) {
    auto regStr = generator.createBinOp(
        LLVMGenerator::BinOpName::ADD,
        toLLVMType(static_cast<PrimitiveType>(left.typeID)), left.name,
        right.name);
    return Value(regStr, left.typeID);
  }

  Value subtractRegisters(const Value& left, const Value& right) {
    auto regStr = generator.createBinOp(
        LLVMGenerator::BinOpName::SUB,
        toLLVMType(static_cast<PrimitiveType>(left.typeID)), left.name,
        right.name);
    return Value(regStr, left.typeID);
  }

  Value multiplyRegisters(const Value& left, const Value& right) {
    auto regStr = generator.createBinOp(
        LLVMGenerator::BinOpName::MUL,
        toLLVMType(static_cast<PrimitiveType>(left.typeID)), left.name,
        right.name);
    return Value(regStr, left.typeID);
  }

  Value divideRegisters(const Value& left, const Value& right) {
    auto regStr = generator.createBinOp(
        LLVMGenerator::BinOpName::DIV,
        toLLVMType(static_cast<PrimitiveType>(left.typeID)), left.name,
        right.name);
    return Value(regStr, left.typeID);
  }

  Value modRegisters(const Value& left, const Value& right) {
    auto regStr = generator.createBinOp(
        LLVMGenerator::BinOpName::SREM,
        toLLVMType(static_cast<PrimitiveType>(left.typeID)), left.name,
        right.name);
    return Value(regStr, left.typeID);
  }

  Value castRegister(const Value& value, int targetType) {
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

  Value negateRegister(const Value& value) {
    auto llvmType = toLLVMType(static_cast<PrimitiveType>(value.typeID));
    auto regStr = generator.createBinOp(LLVMGenerator::BinOpName::SUB, llvmType,
                                        LLVMGenerator::getZeroLiteral(llvmType),
                                        value.name);
    return Value(regStr, value.typeID);
  }

  Value plusRegister(const Value& value) { return value; }

  Value compareRegisters(Value left, Value right, Relation rel) {
    auto regStr =
        generator.createRel(toLLVMType(static_cast<PrimitiveType>(left.typeID)),
                            toLLVMRel(rel), left.name, right.name);
    return Value(regStr, BOOL_TYPE_ID);
  }

  Value load(const Value& val) {
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

  bool applyBuiltInConversions(Value& left, Value& right,
                               const antlr4::ParserRuleContext* ctx) {
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
};
}  // namespace lexy2