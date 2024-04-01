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
  if (value.type == "int") {
    generator.printI32(value.name);
  }
  if (value.type == "double") {
    generator.printDouble(value.name);
  }
}

void TranslatorListener::exitDeclStatement(
    Lexy2Parser::DeclStatementContext* ctx) {
  if (inErrorMode)
    return;

  auto identifier = ctx->IDENTIFIER()->getText();
  auto initializer = valueStack.top();
  if (initializer.category == Value::Category::MEMORY) {
    initializer = load(initializer);
  }
  valueStack.pop();
  if (symbolTable.find(identifier) != symbolTable.end()) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Identifier '" + identifier + "' already in use");
    inErrorMode = true;
    return;
  }
  if (ctx->TYPE_ID() != nullptr) {
    castRegister(initializer, ctx->TYPE_ID()->getText());
    initializer = valueStack.top();
    valueStack.pop();
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
    addRegisters(left, right);
  }
  if (op == "-") {
    subtractRegisters(left, right);
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
    multiplyRegisters(left, right);
  }
  if (op == "/") {
    divideRegisters(left, right);
  }
  if (op == "%") {
    modRegisters(left, right, ctx);
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
  castRegister(value, targetType);
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
    negateRegister(value);
  }
  if (ctx->op->getText() == "+") {
    plusRegister(value);
  }
}

void TranslatorListener::exitIdenitifer(Lexy2Parser::IdenitiferContext* ctx) {
  if (inErrorMode)
    return;

  const auto id = ctx->IDENTIFIER()->getText();
  const auto loc = symbolTable.find(id);
  if (loc != symbolTable.end()) {
    valueStack.push(loc->second);
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

  valueStack.push(Value(ctx->INTEGER_LITERAL()->getText(), "int"));
}

void TranslatorListener::exitFloatLiteral(
    Lexy2Parser::FloatLiteralContext* ctx) {
  if (inErrorMode)
    return;

  valueStack.push(Value(ctx->FLOAT_LITERAL()->getText(), "double"));
}

void TranslatorListener::exitBoolLiteral(Lexy2Parser::BoolLiteralContext* ctx) {
  if (inErrorMode)
    return;

  valueStack.push(Value(ctx->BOOL_LITERAL()->getText(), "bool"));
}
}  // namespace lexy2