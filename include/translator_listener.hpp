#pragma once

#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include "Lexy2BaseListener.h"
#include "llvm_generator.hpp"

struct Variable {
  std::string type;
};

struct Value {
  Value(std::string name, std::string type)
      : name(std::move(name)), type(std::move(type)) {}
  std::string name;
  std::string type;
  enum class Category {
    LLVM_REGISTER,
    LITERAL,
  };
  Category category;
};

class TranslatorListener : public Lexy2BaseListener {
  std::stack<Value> valueStack;
  std::map<std::string, Value> symbolTable;
  LLVMGenerator generator;

 public:
  void exitTranslationUnit(Lexy2Parser::TranslationUnitContext* ctx) override {}

  void exitStatement(Lexy2Parser::StatementContext* ctx) override {}

  void exitExprStatement(Lexy2Parser::ExprStatementContext* ctx) override {}

  void exitPrintIntrinsic(Lexy2Parser::PrintIntrinsicContext* ctx) override {
    auto value = valueStack.top();
    valueStack.pop();
    if (value.type == "int") {
      generator.printI32(value.name);
    }
    if (value.type == "double") {
      generator.printDouble(value.name);
    }
  }

  void exitDeclStatement(Lexy2Parser::DeclStatementContext* ctx) override {}

  void exitComma(Lexy2Parser::CommaContext* ctx) override {}

  void exitTernary(Lexy2Parser::TernaryContext* ctx) override {}

  void exitAssign(Lexy2Parser::AssignContext* ctx) override {}

  void exitLogicalAnd(Lexy2Parser::LogicalAndContext* ctx) override {}

  void exitLogicalOr(Lexy2Parser::LogicalOrContext* ctx) override {}

  void exitEquality(Lexy2Parser::EqualityContext* ctx) override {}

  void exitRelation(Lexy2Parser::RelationContext* ctx) override {}

  void exitAdditive(Lexy2Parser::AdditiveContext* ctx) override {
    auto [left, right] = popTwo();
    auto op = ctx->op->getText();
    if (op == "+") {
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
    if (op == "-") {
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
        subWithRightCast(right, left);
      }
    }
  }

  void exitMultiplicative(Lexy2Parser::MultiplicativeContext* ctx) override {
    auto [left, right] = popTwo();
    auto op = ctx->op->getText();
    if (op == "*") {
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
    if (op == "/") {
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
        divWithRightCast(right, left);
      }
    }
  }

  void exitCast(Lexy2Parser::CastContext* ctx) override {
    const auto value = valueStack.top();
    valueStack.pop();
    const auto targetType = ctx->TYPE_ID()->getText();
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
    const auto value = valueStack.top();
    valueStack.pop();

    std::string regStr;
    if (ctx->op->getText() == "-") {
      if (value.type == "int") {
        regStr = generator.subI32("0", value.name);
        valueStack.push(Value(regStr, "int"));
      } else if (value.type == "double") {
        regStr = generator.subDouble("0", value.name);
        valueStack.push(Value(regStr, "double"));
      }
    }
  }

  void exitIdenitifer(Lexy2Parser::IdenitiferContext* ctx) override {
    const auto id = ctx->IDENTIFIER()->getText();
    const auto loc = symbolTable.find(id);
    if (loc != symbolTable.end()) {
      valueStack.push(loc->second);

    } else {
      //TODO: error
    }
  }

  void exitParens(Lexy2Parser::ParensContext* ctx) override {}

  void exitIntegerLiteral(Lexy2Parser::IntegerLiteralContext* ctx) override {
    valueStack.push(Value(ctx->INTEGER_LITERAL()->getText(), "int"));
  }

  void exitFloatLiteral(Lexy2Parser::FloatLiteralContext* ctx) override {
    valueStack.push(Value(ctx->FLOAT_LITERAL()->getText(), "double"));
  }

  void exitBoolLiteral(Lexy2Parser::BoolLiteralContext* ctx) override {
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

  void mulWithRightCast(Value& left, Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.mulDouble(left.name, tempRegStr);
    valueStack.push(Value(regStr, "double"));
  }

  void divWithRightCast(Value& left, Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.divDouble(left.name, tempRegStr);
    valueStack.push(Value(regStr, "double"));
  }

  void addWithRightCast(Value& left, Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.addDouble(left.name, tempRegStr);
    valueStack.push(Value(regStr, "double"));
  }

  void subWithRightCast(Value& left, Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.subDouble(left.name, tempRegStr);
    valueStack.push(Value(regStr, "double"));
  }
};