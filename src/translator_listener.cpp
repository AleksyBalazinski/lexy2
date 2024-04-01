#include "translator_listener.hpp"

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
  if (initializer.typeID == INT_TYPE_ID) {
    generator.declareI32(identifier + symbolTable.getCurrentScopeID());
    generator.assignI32(identifier + symbolTable.getCurrentScopeID(),
                        initializer.name);
    symbolTable.insertInCurrentScope(std::make_pair(
        identifier, Value(identifier, INT_TYPE_ID, Value::Category::MEMORY)));
  }
  if (initializer.typeID == DOUBLE_TYPE_ID) {
    generator.declareDouble(identifier + symbolTable.getCurrentScopeID());
    generator.assignDouble(identifier + symbolTable.getCurrentScopeID(),
                           initializer.name);
    symbolTable.insertInCurrentScope(std::make_pair(
        identifier,
        Value(identifier, DOUBLE_TYPE_ID, Value::Category::MEMORY)));
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
  const auto [iter, scopeID] = symbolTable.globalFind(identifier);
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
  variable.name += symbolTable.getScopeID(scopeID);

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
    auto res = modRegisters(variable, value, ctx);
    if (res.has_value()) {
      value = *res;
    } else {
      return;
    }
  }
  if (op == "=") {
    if (variable.typeID != value.typeID) {
      value = castRegister(value, variable.typeID);
    }
  }
  if (variable.typeID == DOUBLE_TYPE_ID) {
    generator.assignDouble(identifier, value.name);
  }
  if (variable.typeID == INT_TYPE_ID) {
    generator.assignI32(identifier + symbolTable.getScopeID(scopeID),
                        value.name);
  }
  valueStack.push(value);
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
  auto op = ctx->op->getText();
  if (op == "+") {
    valueStack.push(addRegisters(left, right));
  }
  if (op == "-") {
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

  auto op = ctx->op->getText();
  if (op == "*") {
    valueStack.push(multiplyRegisters(left, right));
  }
  if (op == "/") {
    valueStack.push(divideRegisters(left, right));
  }
  if (op == "%") {
    const auto val = modRegisters(left, right, ctx);
    if (val.has_value()) {
      valueStack.push(*val);
    } else {
      return;
    }
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
  const auto [loc, scopeID] = symbolTable.globalFind(id);
  if (loc != symbolTable.end()) {
    auto scopedIdentifier = loc->second;
    scopedIdentifier.name += symbolTable.getScopeID(scopeID);
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