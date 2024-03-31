#pragma once

#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Lexy2BaseListener.h"
#include "error_handler.hpp"
#include "llvm_generator.hpp"

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

  void exitStatement(Lexy2Parser::StatementContext* ctx) override {
    if (inErrorMode) {  // synchronize
      encounteredErrors = true;
      inErrorMode = false;
      while (!valueStack.empty()) {
        valueStack.pop();
      }
    }
  }

  void exitExprStatement(Lexy2Parser::ExprStatementContext* ctx) override {}

  void exitPrintIntrinsic(Lexy2Parser::PrintIntrinsicContext* ctx) override {
    if (inErrorMode)
      return;

    auto value = valueStack.top();
    valueStack.pop();
    if (value.category == Value::Category::MEMORY) {
      value = load(value);
    }
    if (value.type == "int") {
      generator.printI32(value.name);
    }
    if (value.type == "double") {
      generator.printDouble(value.name);
    }
  }

  void exitDeclStatement(Lexy2Parser::DeclStatementContext* ctx) override {
    if (inErrorMode)
      return;

    auto identifier = ctx->IDENTIFIER()->getText();
    auto initializer = valueStack.top();
    if (initializer.category == Value::Category::MEMORY) {
      initializer = load(initializer);
    }
    valueStack.pop();
    if (symbolTable.find(identifier) != symbolTable.end()) {
      errorHandler.reportError(
          getLineCol(ctx), "Identifier '" + identifier + "' already in use");
      inErrorMode = true;
      return;
    }
    if (ctx->TYPE_ID() != nullptr) {
      if (ctx->TYPE_ID()->getText() == "double") {
        castRegister(initializer, "double");
        initializer = valueStack.top();
        valueStack.pop();
      }
      if (ctx->TYPE_ID()->getText() == "int") {
        castRegister(initializer, "int");
        initializer = valueStack.top();
        valueStack.pop();
      }
    }
    if (initializer.type == "int") {
      generator.declareI32(identifier);
      generator.assignI32(identifier, initializer.name);
      symbolTable.insert(std::make_pair(
          identifier, Value(identifier, "int", Value::Category::MEMORY)));
    }
    if (initializer.type == "double") {
      generator.declareDouble(identifier);
      generator.assignDouble(identifier, initializer.name);
      symbolTable.insert(std::make_pair(
          identifier, Value(identifier, "double", Value::Category::MEMORY)));
    }
  }

  void exitComma(Lexy2Parser::CommaContext* ctx) override {}

  void exitTernary(Lexy2Parser::TernaryContext* ctx) override {}

  void exitAssign(Lexy2Parser::AssignContext* ctx) override {}

  void exitLogicalAnd(Lexy2Parser::LogicalAndContext* ctx) override {}

  void exitLogicalOr(Lexy2Parser::LogicalOrContext* ctx) override {}

  void exitEquality(Lexy2Parser::EqualityContext* ctx) override {}

  void exitRelation(Lexy2Parser::RelationContext* ctx) override {}

  void exitAdditive(Lexy2Parser::AdditiveContext* ctx) override {
    if (inErrorMode)
      return;

    auto [left, right] = popTwo();
    if (left.category == Value::Category::MEMORY) {
      left = load(left);
    }
    if (right.category == Value::Category::MEMORY) {
      right = load(right);
    }
    auto op = ctx->op->getText();
    if (op == "+") {
      addRegisters(left, right);
    }
    if (op == "-") {
      subtractRegisters(left, right);
    }
  }

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
      addWithRightCast(left, right);
    }
    if (left.type == "int" && right.type == "double") {
      addWithRightCast(right, left);
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
      subWithRightCast(left, right);
    }
    if (left.type == "int" && right.type == "double") {
      subWithLeftCast(left, right);
    }
  }

  void exitMultiplicative(Lexy2Parser::MultiplicativeContext* ctx) override {
    if (inErrorMode)
      return;

    auto [left, right] = popTwo();
    if (left.category == Value::Category::MEMORY) {
      left = load(left);
    }
    if (right.category == Value::Category::MEMORY) {
      right = load(right);
    }

    auto op = ctx->op->getText();
    if (op == "*") {
      multiplyRegisters(left, right);
    }
    if (op == "/") {
      divideRegisters(left, right);
    }
    if (op == "%") {
      modRegisters(left, right, ctx);
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
      mulWithRightCast(left, right);
    }
    if (left.type == "int" && right.type == "double") {
      mulWithRightCast(right, left);
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
      divWithRightCast(left, right);
    }
    if (left.type == "int" && right.type == "double") {
      divWithLeftCast(left, right);
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

  void exitCast(Lexy2Parser::CastContext* ctx) override {
    if (inErrorMode)
      return;

    auto value = valueStack.top();
    valueStack.pop();
    if (value.category == Value::Category::MEMORY) {
      value = load(value);
    }
    const auto targetType = ctx->TYPE_ID()->getText();
    castRegister(value, targetType);
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

  void exitUnary(Lexy2Parser::UnaryContext* ctx) override {
    if (inErrorMode)
      return;

    auto value = valueStack.top();
    valueStack.pop();
    if (value.category == Value::Category::MEMORY) {
      value = load(value);
    }

    if (ctx->op->getText() == "-") {
      negateRegister(value);
    }
    if (ctx->op->getText() == "+") {
      plusRegister(value);
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

  void exitIdenitifer(Lexy2Parser::IdenitiferContext* ctx) override {
    if (inErrorMode)
      return;

    const auto id = ctx->IDENTIFIER()->getText();
    const auto loc = symbolTable.find(id);
    if (loc != symbolTable.end()) {
      valueStack.push(loc->second);
    } else {
      errorHandler.reportError(getLineCol(ctx),
                               "Identifier '" + id + "' not declared");
      inErrorMode = true;
    }
  }

  void exitParens(Lexy2Parser::ParensContext* ctx) override {}

  void exitIntegerLiteral(Lexy2Parser::IntegerLiteralContext* ctx) override {
    if (inErrorMode)
      return;

    valueStack.push(Value(ctx->INTEGER_LITERAL()->getText(), "int"));
  }

  void exitFloatLiteral(Lexy2Parser::FloatLiteralContext* ctx) override {
    if (inErrorMode)
      return;

    valueStack.push(Value(ctx->FLOAT_LITERAL()->getText(), "double"));
  }

  void exitBoolLiteral(Lexy2Parser::BoolLiteralContext* ctx) override {
    if (inErrorMode)
      return;

    valueStack.push(Value(ctx->BOOL_LITERAL()->getText(), "bool"));
  }

  std::string getCode(const std::string& sourceFilename) {
    return generator.emitCode(sourceFilename);
  }

 private:
  std::pair<Value, Value> popTwo() {
    auto right = valueStack.top();
    valueStack.pop();
    auto left = valueStack.top();
    valueStack.pop();
    return std::make_pair(left, right);
  }

  void mulWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.mulDouble(left.name, tempRegStr);
    valueStack.push(Value(regStr, "double"));
  }

  void divWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.divDouble(left.name, tempRegStr);
    valueStack.push(Value(regStr, "double"));
  }

  void divWithLeftCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(left.name);
    auto regStr = generator.divDouble(tempRegStr, right.name);
    valueStack.push(Value(regStr, "double"));
  }

  void addWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.addDouble(left.name, tempRegStr);
    valueStack.push(Value(regStr, "double"));
  }

  void subWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.subDouble(left.name, tempRegStr);
    valueStack.push(Value(regStr, "double"));
  }

  void subWithLeftCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(left.name);
    auto regStr = generator.subDouble(tempRegStr, right.name);
    valueStack.push(Value(regStr, "double"));
  }

  Value load(const Value& val) {
    if (val.type == "double") {
      return Value(generator.loadDouble(val.name), "double");
    }
    if (val.type == "int") {
      return Value(generator.loadI32(val.name), "int");
    }
  }

  std::pair<int, int> getLineCol(antlr4::ParserRuleContext* ctx) {
    auto tok = ctx->getStart();
    return std::make_pair<int, int>(tok->getLine(),
                                    tok->getCharPositionInLine());
  }
};