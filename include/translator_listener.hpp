#pragma once

#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Lexy2BaseListener.h"
#include "error_handler.hpp"
#include "llvm_generator.hpp"
#include "utils.hpp"

struct Value {
  enum class Category { REGISTER, MEMORY, CONSTANT };
  Value(std::string name, std::string type, Category category)
      : name(std::move(name)), type(std::move(type)), category(category) {}

  Value(std::string name, std::string type)
      : Value(name, type, Category::REGISTER) {}

  std::string name;
  std::string type;
  Category category;
};

class TranslatorListener : public Lexy2BaseListener {
  std::stack<Value> valueStack;
  std::map<std::string, Value> symbolTable;
  LLVMGenerator generator;

  ErrorHandler& errorHandler;
  bool inErrorMode = false;
  bool encounteredErrors = false;

 public:
  TranslatorListener(ErrorHandler& errorHandler) : errorHandler(errorHandler) {}
  void exitTranslationUnit(Lexy2Parser::TranslationUnitContext* ctx) override {}

  void exitStatement(Lexy2Parser::StatementContext* ctx) override;

  void exitExprStatement(Lexy2Parser::ExprStatementContext* ctx) override {}

  void exitPrintIntrinsic(Lexy2Parser::PrintIntrinsicContext* ctx) override;

  void exitDeclStatement(Lexy2Parser::DeclStatementContext* ctx) override;

  void exitComma(Lexy2Parser::CommaContext* ctx) override {}

  void exitTernary(Lexy2Parser::TernaryContext* ctx) override {}

  void exitAssign(Lexy2Parser::AssignContext* ctx) override {}

  void exitLogicalAnd(Lexy2Parser::LogicalAndContext* ctx) override {}

  void exitLogicalOr(Lexy2Parser::LogicalOrContext* ctx) override {}

  void exitEquality(Lexy2Parser::EqualityContext* ctx) override {}

  void exitRelation(Lexy2Parser::RelationContext* ctx) override {}

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
  void addRegisters(const Value& left, const Value& right) {
    if (left.type == "int" && right.type == "int") {
      auto regStr = generator.addI32(left.name, right.name);
      valueStack.push(Value(regStr, "int"));
    }
    if (left.type == "double" && right.type == "double") {
      auto regStr = generator.addDouble(left.name, right.name);
      valueStack.push(Value(regStr, "double"));
    }
    if (left.type == "double" && right.type == "int") {
      valueStack.push(addWithRightCast(left, right));
    }
    if (left.type == "int" && right.type == "double") {
      valueStack.push(addWithRightCast(right, left));
    }
  }

  void subtractRegisters(const Value& left, const Value& right) {
    if (left.type == "int" && right.type == "int") {
      auto regStr = generator.subI32(left.name, right.name);
      valueStack.push(Value(regStr, "int"));
    }
    if (left.type == "double" && right.type == "double") {
      auto regStr = generator.subDouble(left.name, right.name);
      valueStack.push(Value(regStr, "double"));
    }
    if (left.type == "double" && right.type == "int") {
      valueStack.push(subWithRightCast(left, right));
    }
    if (left.type == "int" && right.type == "double") {
      valueStack.push(subWithLeftCast(left, right));
    }
  }

  void multiplyRegisters(const Value& left, const Value& right) {
    if (left.type == "int" && right.type == "int") {
      auto regStr = generator.mulI32(left.name, right.name);
      valueStack.push(Value(regStr, "int"));
    }
    if (left.type == "double" && right.type == "double") {
      auto regStr = generator.mulDouble(left.name, right.name);
      valueStack.push(Value(regStr, "double"));
    }
    if (left.type == "double" && right.type == "int") {
      valueStack.push(mulWithRightCast(left, right));
    }
    if (left.type == "int" && right.type == "double") {
      valueStack.push(mulWithRightCast(right, left));
    }
  }

  void divideRegisters(const Value& left, const Value& right) {
    if (left.type == "int" && right.type == "int") {
      auto regStr = generator.divI32(left.name, right.name);
      valueStack.push(Value(regStr, "int"));
    }
    if (left.type == "double" && right.type == "double") {
      auto regStr = generator.divDouble(left.name, right.name);
      valueStack.push(Value(regStr, "double"));
    }
    if (left.type == "double" && right.type == "int") {
      valueStack.push(divWithRightCast(left, right));
    }
    if (left.type == "int" && right.type == "double") {
      valueStack.push(divWithLeftCast(left, right));
    }
  }

  void modRegisters(const Value& left, const Value& right,
                    antlr4::ParserRuleContext* ctx) {
    if (left.type == "int" && right.type == "int") {
      auto regStr = generator.remI32(left.name, right.name);
      valueStack.push(Value(regStr, "int"));
    } else {
      auto pos = getLineCol(ctx);
      errorHandler.reportError(
          pos.first, pos.second,
          "Operator % can only be applioed to int operands");
      inErrorMode = true;
    }
  }

  void castRegister(const Value& value, const std::string& targetType) {
    if (value.type == targetType) {
      valueStack.push(value);  // do nothing
    }
    if (value.type == "int") {
      if (targetType == "double") {
        auto regStr = generator.castI32ToDouble(value.name);
        valueStack.push(Value(regStr, "double"));
      }
    }
    if (value.type == "double") {
      if (targetType == "int") {
        auto regStr = generator.castDoubleToI32(value.name);
        valueStack.push(Value(regStr, "int"));
      }
    }
  }

  void negateRegister(const Value& value) {
    std::string regStr;
    if (value.type == "int") {
      regStr = generator.subI32("0", value.name);
      valueStack.push(Value(regStr, "int"));
    } else if (value.type == "double") {
      regStr = generator.subDouble("0.0", value.name);
      valueStack.push(Value(regStr, "double"));
    }
  }

  void plusRegister(const Value& value) { valueStack.push(value); }

  Value mulWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.mulDouble(left.name, tempRegStr);
    return Value(regStr, "double");
  }

  Value divWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.divDouble(left.name, tempRegStr);
    return Value(regStr, "double");
  }

  Value divWithLeftCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(left.name);
    auto regStr = generator.divDouble(tempRegStr, right.name);
    return Value(regStr, "double");
  }

  Value addWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.addDouble(left.name, tempRegStr);
    return Value(regStr, "double");
  }

  Value subWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.subDouble(left.name, tempRegStr);
    return Value(regStr, "double");
  }

  Value subWithLeftCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(left.name);
    auto regStr = generator.subDouble(tempRegStr, right.name);
    return Value(regStr, "double");
  }

  Value load(const Value& val) {
    if (val.type == "double") {
      return Value(generator.loadDouble(val.name), "double");
    }
    if (val.type == "int") {
      return Value(generator.loadI32(val.name), "int");
    }
  }
};