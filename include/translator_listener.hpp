#pragma once

#include <optional>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Lexy2BaseListener.h"
#include "code_gen/llvm_generator.hpp"
#include "error.hpp"
#include "error_handler.hpp"
#include "function_param.hpp"
#include "operations.hpp"
#include "symbol_table.hpp"
#include "type_manager.hpp"
#include "types/type_node_factory.hpp"

namespace lexy2 {

class TranslatorListener : public Lexy2BaseListener {
  std::stack<Value> valueStack;
  std::stack<std::string> basicBlockStack;
  std::stack<std::string> returnPointsStack;
  std::unique_ptr<types::TypeNode> currTypeNode;
  std::stack<std::vector<FunctionParam>>
      functionParams;                     // TODO move it somewhere
  std::stack<types::Type> retTypesStack;  // TODO move it somewhere
  std::stack<std::string> functionNames;  // TODO move it somewhere
  std::vector<std::string> fieldNames;
  std::vector<types::Type> fieldTypes;
  int functionArgsCount = 0;
  std::stack<int> rankSpecStack;
  SymbolTable symbolTable;
  LLVMGenerator generator;
  types::TypeNodeFactory typeNodeFactory;
  TypeManager typeManager;

  ErrorHandler& errorHandler;
  bool inErrorMode = false;

 public:
  TranslatorListener(ErrorHandler& errorHandler);

  void exitTranslationUnit(Lexy2Parser::TranslationUnitContext* ctx) override {}

  void exitStatement(Lexy2Parser::StatementContext* ctx) override;

  void exitExprStatement(Lexy2Parser::ExprStatementContext* ctx) override {}

  void exitPrintIntrinsic(Lexy2Parser::PrintIntrinsicContext* ctx) override;

  void exitReadIntrinsic(Lexy2Parser::ReadIntrinsicContext* ctx) override;

  void exitVariableDeclaration(
      Lexy2Parser::VariableDeclarationContext* ctx) override;

  void enterFunctionDeclaration(
      Lexy2Parser::FunctionDeclarationContext* ctx) override;
  void exitFunctionDeclaration(
      Lexy2Parser::FunctionDeclarationContext* ctx) override;

  void exitStructDeclaration(
      Lexy2Parser::StructDeclarationContext* ctx) override;

  void exitStructField(Lexy2Parser::StructFieldContext* ctx) override;

  void exitFunctionName(Lexy2Parser::FunctionNameContext* ctx) override;

  void exitReturnType(Lexy2Parser::ReturnTypeContext* ctx) override;

  void exitParam(Lexy2Parser::ParamContext* ctx) override;

  void enterFunctionBody(Lexy2Parser::FunctionBodyContext* ctx) override;
  void exitFunctionBody(Lexy2Parser::FunctionBodyContext* ctx) override;

  void exitReturnStatement(Lexy2Parser::ReturnStatementContext* ctx) override;

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

  void enterWhileLoopBody(Lexy2Parser::WhileLoopBodyContext* ctx) override;
  void exitWhileLoopBody(Lexy2Parser::WhileLoopBodyContext* ctx) override;

  void enterWhileLoopCondition(
      Lexy2Parser::WhileLoopConditionContext* ctx) override;
  void exitWhileLoopCondition(
      Lexy2Parser::WhileLoopConditionContext* ctx) override;

  void enterWhileLoop(Lexy2Parser::WhileLoopContext* ctx) override;
  void exitWhileLoop(Lexy2Parser::WhileLoopContext* ctx) override;

  void exitComma(Lexy2Parser::CommaContext* ctx) override {}

  void exitTernary(Lexy2Parser::TernaryContext* ctx) override {}

  void exitAssign(Lexy2Parser::AssignContext* ctx) override;

  void exitLogicalAnd(Lexy2Parser::LogicalAndContext* ctx) override;

  void exitLogicalOr(Lexy2Parser::LogicalOrContext* ctx) override;

  void exitLogicalXor(Lexy2Parser::LogicalXorContext* ctx) override;

  void exitEquality(Lexy2Parser::EqualityContext* ctx) override;

  void exitRelation(Lexy2Parser::RelationContext* ctx) override;

  void exitAdditive(Lexy2Parser::AdditiveContext* ctx) override;

  void exitMultiplicative(Lexy2Parser::MultiplicativeContext* ctx) override;

  void exitCast(Lexy2Parser::CastContext* ctx) override;

  void exitUnary(Lexy2Parser::UnaryContext* ctx) override;

  void exitIdentifier(Lexy2Parser::IdentifierContext* ctx) override;

  void exitParens(Lexy2Parser::ParensContext* ctx) override {}

  void exitIntegerLiteral(Lexy2Parser::IntegerLiteralContext* ctx) override;

  void exitFloatLiteral(Lexy2Parser::FloatLiteralContext* ctx) override;

  void exitBoolLiteral(Lexy2Parser::BoolLiteralContext* ctx) override;

  void exitElementIndex(Lexy2Parser::ElementIndexContext* ctx) override;

  void exitType(Lexy2Parser::TypeContext* ctx) override {}

  void exitStructAccess(Lexy2Parser::StructAccessContext* ctx) override;

  void enterFunctionCall(Lexy2Parser::FunctionCallContext* ctx) override;
  void exitFunctionCall(Lexy2Parser::FunctionCallContext* ctx) override;

  void exitFunctionArg(Lexy2Parser::FunctionArgContext* ctx) override;

  void exitArrayType(Lexy2Parser::ArrayTypeContext* ctx) override;

  void exitRankSpecifier(Lexy2Parser::RankSpecifierContext* ctx) override;

  void exitSimpleType(Lexy2Parser::SimpleTypeContext* ctx) override;

  void exitReferenceType(Lexy2Parser::ReferenceTypeContext* ctx) override;

  std::string getCode(const std::string& sourceFilename);

 private:
  Value addRegisters(const Value& left, const Value& right);

  Value subtractRegisters(const Value& left, const Value& right);

  Value multiplyRegisters(const Value& left, const Value& right);

  Value divideRegisters(const Value& left, const Value& right);

  Value modRegisters(const Value& left, const Value& right);

  Value castRegister(const Value& value, const types::Type& targetType);

  Value negateRegister(const Value& value);

  Value plusRegister(const Value& value);

  Value compareRegisters(Value left, Value right, Relation rel);

  Value load(const Value& val);

  bool applyBuiltInConversions(Value& left, Value& right,
                               const antlr4::ParserRuleContext* ctx);

  void assignToVariable(const Value& lhs, Value& value,
                        Lexy2Parser::AssignContext* ctx);
  void assignToInternalPtr(const Value& lhs, Value& value,
                           Lexy2Parser::AssignContext* ctx);

  Error<std::string> declareVariable(
      const std::string& identifier, const Value& intiailizer,
      const std::optional<types::Type> declaredType);

  Error<std::string> declareFunction(
      const std::string& functionName, const types::Type& retType,
      const std::vector<FunctionParam>& functionParams);
};
}  // namespace lexy2