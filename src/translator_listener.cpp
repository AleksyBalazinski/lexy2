#include "translator_listener.hpp"
#include <tuple>

namespace lexy2 {
void TranslatorListener::exitStatement(Lexy2Parser::StatementContext* ctx) {
  if (inErrorMode) {  // synchronize
    encounteredErrors = true;
    inErrorMode = false;
    while (!valueStack.empty()) {
      valueStack.pop();
    }
  }
}

void TranslatorListener::exitPrintIntrinsic(
    Lexy2Parser::PrintIntrinsicContext* ctx) {
  if (inErrorMode)
    return;

  auto value = valueStack.top();
  valueStack.pop();
  if (value.category == Value::Category::MEMORY) {
    value = load(value);
  }
  if (value.typeID == INT_TYPE_ID) {
    generator.printI32(value.name);
  }
  if (value.typeID == DOUBLE_TYPE_ID) {
    generator.printDouble(value.name);
  }
  if (value.typeID == BOOL_TYPE_ID) {
    const auto casted = generator.castBoolToI32(value.name);
    generator.printI32(casted);
  }
}

void TranslatorListener::exitDeclStatement(
    Lexy2Parser::DeclStatementContext* ctx) {
  if (inErrorMode)
    return;

  auto identifier = ctx->IDENTIFIER()->getText();
  auto initializer = valueStack.top();
  valueStack.pop();
  if (initializer.category == Value::Category::MEMORY) {
    initializer = load(initializer);
  }
  if (symbolTable.currentScopeFind(identifier) !=
      symbolTable.getCurrentScope().end()) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Identifier '" + identifier + "' already in use");
    inErrorMode = true;
    return;
  }
  if (ctx->TYPE_ID() != nullptr) {
    initializer = castRegister(initializer, typeIDs[ctx->TYPE_ID()->getText()]);
  }
  const auto scopedIdentifier = identifier + symbolTable.getCurrentScopeID();
  if (initializer.typeID == INT_TYPE_ID) {
    generator.createDeclaration(LLVMGenerator::Type::I32, scopedIdentifier);
    generator.createAssignment(LLVMGenerator::Type::I32, scopedIdentifier,
                               initializer.name);
    symbolTable.insertInCurrentScope(std::make_pair(
        identifier, Value(identifier, INT_TYPE_ID, Value::Category::MEMORY)));
  }
  if (initializer.typeID == DOUBLE_TYPE_ID) {
    generator.createDeclaration(LLVMGenerator::Type::DOUBLE, scopedIdentifier);
    generator.createAssignment(LLVMGenerator::Type::DOUBLE, scopedIdentifier,
                               initializer.name);
    symbolTable.insertInCurrentScope(std::make_pair(
        identifier,
        Value(identifier, DOUBLE_TYPE_ID, Value::Category::MEMORY)));
  }
  if (initializer.typeID == BOOL_TYPE_ID) {
    initializer.name = generator.castI1toI8(initializer.name);
    generator.createDeclaration(LLVMGenerator::Type::I8, scopedIdentifier);
    generator.createAssignment(LLVMGenerator::Type::I8, scopedIdentifier,
                               initializer.name);
    symbolTable.insertInCurrentScope(std::make_pair(
        identifier, Value(identifier, BOOL_TYPE_ID, Value::Category::MEMORY)));
  }
}

void TranslatorListener::enterCompoundStatement(
    Lexy2Parser::CompoundStatementContext* ctx) {
  symbolTable.enterNewScope();
}

void TranslatorListener::exitCompoundStatement(
    Lexy2Parser::CompoundStatementContext* ctx) {
  symbolTable.leaveScope();
}

void TranslatorListener::exitAssign(Lexy2Parser::AssignContext* ctx) {
  if (inErrorMode)
    return;

  const auto identifier = ctx->IDENTIFIER()->getText();
  const auto [iter, depth] = symbolTable.globalFind(identifier);
  if (iter == symbolTable.end()) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Identifier '" + identifier + "' not declared");
    inErrorMode = true;
    return;
  }
  auto value = valueStack.top();
  valueStack.pop();
  if (value.category == Value::Category::MEMORY) {
    value = load(value);
  }
  auto variable = iter->second;
  variable.name += symbolTable.getScopeID(depth);

  if (variable.typeID != value.typeID) {
    if (typeManager.isImplicitFromTo(value.typeID, variable.typeID)) {
      value = castRegister(value, variable.typeID);
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
  if (variable.typeID == DOUBLE_TYPE_ID) {
    generator.createAssignment(LLVMGenerator::Type::DOUBLE, scopedIdentifier,
                               value.name);
  }
  if (variable.typeID == INT_TYPE_ID) {
    generator.createAssignment(LLVMGenerator::Type::I32, scopedIdentifier,
                               value.name);
  }
  if (variable.typeID == BOOL_TYPE_ID) {
    value.name = generator.castI1toI8(value.name);
    generator.createAssignment(LLVMGenerator::Type::I8, scopedIdentifier,
                               value.name);
  }
  valueStack.push(value);
}

void TranslatorListener::exitEquality(Lexy2Parser::EqualityContext* ctx) {
  if (inErrorMode)
    return;

  auto [left, right] = utils::popTwo(valueStack);
  if (left.category == Value::Category::MEMORY) {
    left = load(left);
  }
  if (right.category == Value::Category::MEMORY) {
    right = load(right);
  }
  auto op = ctx->op->getText();
  if (op == "==") {
    valueStack.push(compareRegisters(left, right, Relation::EQ));
  }
  if (op == "!=") {
    valueStack.push(compareRegisters(left, right, Relation::NEQ));
  }
}

void TranslatorListener::exitRelation(Lexy2Parser::RelationContext* ctx) {
  if (inErrorMode)
    return;

  auto [left, right] = utils::popTwo(valueStack);
  if (left.category == Value::Category::MEMORY) {
    left = load(left);
  }
  if (right.category == Value::Category::MEMORY) {
    right = load(right);
  }
  auto op = ctx->op->getText();
  if (op == "<") {
    valueStack.push(compareRegisters(left, right, Relation::LT));
  }
  if (op == ">") {
    valueStack.push(compareRegisters(left, right, Relation::GT));
  }
  if (op == "<=") {
    valueStack.push(compareRegisters(left, right, Relation::LE));
  }
  if (op == ">=") {
    valueStack.push(compareRegisters(left, right, Relation::GE));
  }
}

void TranslatorListener::exitAdditive(Lexy2Parser::AdditiveContext* ctx) {
  if (inErrorMode)
    return;

  auto [left, right] = utils::popTwo(valueStack);
  if (left.category == Value::Category::MEMORY) {
    left = load(left);
  }
  if (right.category == Value::Category::MEMORY) {
    right = load(right);
  }

  if (left.typeID != right.typeID) {
    if (!applyBuiltInConversions(left, right, ctx)) {
      inErrorMode = true;
      return;
    }
  }

  auto op = ctx->op->getText();
  if (op == "+") {
    if (!typeManager.isOperatorSupported(Operator::ADD, left.typeID)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '+' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(addRegisters(left, right));
  }
  if (op == "-") {
    if (!typeManager.isOperatorSupported(Operator::SUB, left.typeID)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '-' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(subtractRegisters(left, right));
  }
}

void TranslatorListener::exitMultiplicative(
    Lexy2Parser::MultiplicativeContext* ctx) {
  if (inErrorMode)
    return;

  auto [left, right] = utils::popTwo(valueStack);
  if (left.category == Value::Category::MEMORY) {
    left = load(left);
  }
  if (right.category == Value::Category::MEMORY) {
    right = load(right);
  }

  if (left.typeID != right.typeID) {
    if (!applyBuiltInConversions(left, right, ctx)) {
      inErrorMode = true;
      return;
    }
  }

  auto op = ctx->op->getText();
  if (op == "*") {
    if (!typeManager.isOperatorSupported(Operator::MUL, left.typeID)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '*' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(multiplyRegisters(left, right));
  }
  if (op == "/") {
    if (!typeManager.isOperatorSupported(Operator::DIV, left.typeID)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '/' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(divideRegisters(left, right));
  }
  if (op == "%") {
    if (!typeManager.isOperatorSupported(Operator::REM, left.typeID)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '%' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(modRegisters(left, right));
  }
}

void TranslatorListener::exitCast(Lexy2Parser::CastContext* ctx) {
  if (inErrorMode)
    return;

  auto value = valueStack.top();
  valueStack.pop();
  if (value.category == Value::Category::MEMORY) {
    value = load(value);
  }
  const auto targetType = ctx->TYPE_ID()->getText();
  valueStack.push(castRegister(value, typeIDs[targetType]));
}

void TranslatorListener::exitUnary(Lexy2Parser::UnaryContext* ctx) {
  if (inErrorMode)
    return;

  auto value = valueStack.top();
  valueStack.pop();
  if (value.category == Value::Category::MEMORY) {
    value = load(value);
  }

  if (ctx->op->getText() == "-") {
    valueStack.push(negateRegister(value));
  }
  if (ctx->op->getText() == "+") {
    valueStack.push(plusRegister(value));
  }
}

void TranslatorListener::exitIdenitifer(Lexy2Parser::IdenitiferContext* ctx) {
  if (inErrorMode)
    return;

  const auto id = ctx->IDENTIFIER()->getText();
  const auto [loc, depth] = symbolTable.globalFind(id);
  if (loc != symbolTable.end()) {
    auto scopedIdentifier = loc->second;
    scopedIdentifier.name += symbolTable.getScopeID(depth);
    valueStack.push(scopedIdentifier);
  } else {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Identifier '" + id + "' not declared");
    inErrorMode = true;
  }
}

void TranslatorListener::exitIntegerLiteral(
    Lexy2Parser::IntegerLiteralContext* ctx) {
  if (inErrorMode)
    return;

  valueStack.push(Value(ctx->INTEGER_LITERAL()->getText(), INT_TYPE_ID));
}

void TranslatorListener::exitFloatLiteral(
    Lexy2Parser::FloatLiteralContext* ctx) {
  if (inErrorMode)
    return;

  valueStack.push(Value(ctx->FLOAT_LITERAL()->getText(), DOUBLE_TYPE_ID));
}

void TranslatorListener::exitBoolLiteral(Lexy2Parser::BoolLiteralContext* ctx) {
  if (inErrorMode)
    return;

  valueStack.push(Value(ctx->BOOL_LITERAL()->getText(), BOOL_TYPE_ID));
}
}  // namespace lexy2