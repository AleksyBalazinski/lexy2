#pragma once

#include <optional>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>

#include "Lexy2BaseListener.h"
#include "error_handler.hpp"
#include "llvm_generator.hpp"
#include "symbol_table.hpp"
#include "utils.hpp"

namespace lexy2 {

class TranslatorListener : public Lexy2BaseListener {
  std::stack<Value> valueStack;
  SymbolTable symbolTable;
  LLVMGenerator generator;
  const int INT_TYPE_ID = 0;
  const int DOUBLE_TYPE_ID = 1;
  const int BOOL_TYPE_ID = 2;
  std::unordered_map<std::string, int> typeIDs;

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
    if (left.typeID == INT_TYPE_ID && right.typeID == INT_TYPE_ID) {
      auto regStr = generator.addI32(left.name, right.name);
      return Value(regStr, INT_TYPE_ID);
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      auto regStr = generator.addDouble(left.name, right.name);
      return Value(regStr, DOUBLE_TYPE_ID);
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == INT_TYPE_ID) {
      return addWithRightCast(left, right);
    }
    if (left.typeID == INT_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      return addWithRightCast(right, left);
    }
    throw std::exception();  // TODO: user defined types
  }

  Value subtractRegisters(const Value& left, const Value& right) {
    if (left.typeID == INT_TYPE_ID && right.typeID == INT_TYPE_ID) {
      auto regStr = generator.subI32(left.name, right.name);
      return Value(regStr, INT_TYPE_ID);
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      auto regStr = generator.subDouble(left.name, right.name);
      return Value(regStr, DOUBLE_TYPE_ID);
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == INT_TYPE_ID) {
      return subWithRightCast(left, right);
    }
    if (left.typeID == INT_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      return subWithLeftCast(left, right);
    }
    throw std::exception();  // TODO: user defined types
  }

  Value multiplyRegisters(const Value& left, const Value& right) {
    if (left.typeID == INT_TYPE_ID && right.typeID == INT_TYPE_ID) {
      auto regStr = generator.mulI32(left.name, right.name);
      return Value(regStr, INT_TYPE_ID);
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      auto regStr = generator.mulDouble(left.name, right.name);
      return Value(regStr, DOUBLE_TYPE_ID);
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == INT_TYPE_ID) {
      return mulWithRightCast(left, right);
    }
    if (left.typeID == INT_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      return mulWithRightCast(right, left);
    }
    throw std::exception();  // TODO: user defined types
  }

  Value divideRegisters(const Value& left, const Value& right) {
    if (left.typeID == INT_TYPE_ID && right.typeID == INT_TYPE_ID) {
      auto regStr = generator.divI32(left.name, right.name);
      return Value(regStr, INT_TYPE_ID);
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      auto regStr = generator.divDouble(left.name, right.name);
      return Value(regStr, DOUBLE_TYPE_ID);
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == INT_TYPE_ID) {
      return divWithRightCast(left, right);
    }
    if (left.typeID == INT_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      return divWithLeftCast(left, right);
    }
    throw std::exception();  // TODO: user defined types
  }

  std::optional<Value> modRegisters(
      const Value& left, const Value& right,
      antlr4::ParserRuleContext*
          ctx) {  // TODO: check if conversion is valid before entering
    if (left.typeID == INT_TYPE_ID && right.typeID == INT_TYPE_ID) {
      auto regStr = generator.remI32(left.name, right.name);
      return Value(regStr, INT_TYPE_ID);
    } else {
      auto pos = utils::getLineCol(ctx);
      errorHandler.reportError(
          pos.first, pos.second,
          "Operator '%' can only be applied to int operands");
      inErrorMode = true;
      return {};
    }
    throw std::exception();  // TODO: user defined types
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
    throw std::exception();  // TODO: user defined types
  }

  Value negateRegister(const Value& value) {
    std::string regStr;
    if (value.typeID == INT_TYPE_ID) {
      regStr = generator.subI32("0", value.name);
      return Value(regStr, INT_TYPE_ID);
    } else if (value.typeID == DOUBLE_TYPE_ID) {
      regStr = generator.subDouble("0.0", value.name);
      return Value(regStr, DOUBLE_TYPE_ID);
    }
    throw std::exception();  // TODO: user defined types
  }

  Value plusRegister(const Value& value) { return value; }

  Value mulWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.mulDouble(left.name, tempRegStr);
    return Value(regStr, DOUBLE_TYPE_ID);
  }

  Value divWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.divDouble(left.name, tempRegStr);
    return Value(regStr, DOUBLE_TYPE_ID);
  }

  Value divWithLeftCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(left.name);
    auto regStr = generator.divDouble(tempRegStr, right.name);
    return Value(regStr, DOUBLE_TYPE_ID);
  }

  Value addWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.addDouble(left.name, tempRegStr);
    return Value(regStr, DOUBLE_TYPE_ID);
  }

  Value subWithRightCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(right.name);
    auto regStr = generator.subDouble(left.name, tempRegStr);
    return Value(regStr, DOUBLE_TYPE_ID);
  }

  Value subWithLeftCast(const Value& left, const Value& right) {
    auto tempRegStr = generator.castI32ToDouble(left.name);
    auto regStr = generator.subDouble(tempRegStr, right.name);
    return Value(regStr, DOUBLE_TYPE_ID);
  }

  Value compareRegisters(Value left, Value right, Relation rel) {
    if (left.typeID == INT_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      left.name = generator.castI32ToDouble(left.name);
      left.typeID = DOUBLE_TYPE_ID;
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == INT_TYPE_ID) {
      right.name = generator.castI32ToDouble(right.name);
      right.typeID = DOUBLE_TYPE_ID;
    }
    if (left.typeID == INT_TYPE_ID && right.typeID == INT_TYPE_ID) {
      std::string regStr;
      switch (rel) {
        case Relation::EQ:
          regStr = generator.cmpEqI32(left.name, right.name);
          break;
        case Relation::NEQ:
          regStr = generator.cmpNeqI32(left.name, right.name);
          break;
        case Relation::GE:
          regStr = generator.cmpGeqI32(left.name, right.name);
          break;
        case Relation::GT:
          regStr = generator.cmpGreaterThanI32(left.name, right.name);
          break;
        case Relation::LE:
          regStr = generator.cmpLeqI32(left.name, right.name);
          break;
        case Relation::LT:
          regStr = generator.cmpLessThanI32(left.name, right.name);
          break;
      }
      return Value(regStr, BOOL_TYPE_ID);
    }
    if (left.typeID == DOUBLE_TYPE_ID && right.typeID == DOUBLE_TYPE_ID) {
      std::string regStr;
      switch (rel) {
        case Relation::EQ:
          regStr = generator.cmpEqDouble(left.name, right.name);
          break;
        case Relation::NEQ:
          regStr = generator.cmpNeqDouble(left.name, right.name);
          break;
        case Relation::GE:
          regStr = generator.cmpGeqDouble(left.name, right.name);
          break;
        case Relation::GT:
          regStr = generator.cmpGreaterThanDouble(left.name, right.name);
          break;
        case Relation::LE:
          regStr = generator.cmpLeqDouble(left.name, right.name);
          break;
        case Relation::LT:
          regStr = generator.cmpLessThanDouble(left.name, right.name);
          break;
      }
      return Value(regStr, BOOL_TYPE_ID);
    }
    throw std::exception();
  }

  Value load(const Value& val) {
    if (val.typeID == DOUBLE_TYPE_ID) {
      return Value(generator.loadDouble(val.name), DOUBLE_TYPE_ID);
    }
    if (val.typeID == INT_TYPE_ID) {
      return Value(generator.loadI32(val.name), INT_TYPE_ID);
    }
    throw std::exception();
  }
};
}  // namespace lexy2