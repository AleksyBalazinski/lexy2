#include "translator_listener.hpp"
#include <tuple>
#include "types/type.hpp"

namespace lexy2 {
TranslatorListener::TranslatorListener(ErrorHandler& errorHandler)
    : errorHandler(errorHandler) {
  typeIDs.insert(std::make_pair("int", INT_TYPE_ID));
  typeIDs.insert(std::make_pair("double", DOUBLE_TYPE_ID));
  typeIDs.insert(std::make_pair("bool", BOOL_TYPE_ID));
  typeIDs.insert(std::make_pair("float", FLOAT_TYPE_ID));
}

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

  auto value = std::move(valueStack.top());
  valueStack.pop();
  if (value.category == Value::Category::MEMORY) {
    value = load(value);
  }
  if (value.type.isLeaf()) {
    const types::TypeNode& type = value.type.getRoot();
    int typeID = *type.getSimpleTypeId();
    if (typeID == INT_TYPE_ID) {
      generator.printI32(value.name);
    }
    if (typeID == DOUBLE_TYPE_ID) {
      generator.printDouble(value.name);
    }
    if (typeID == BOOL_TYPE_ID) {
      const auto cast = generator.extBoolToI32(value.name);
      generator.printI32(cast);
    }
    if (typeID == FLOAT_TYPE_ID) {
      auto cast = generator.extFloatToDouble(value.name);
      generator.printDouble(cast);
    }
  }
}

void TranslatorListener::exitDeclStatement(
    Lexy2Parser::DeclStatementContext* ctx) {
  if (inErrorMode)
    return;

  auto identifier = ctx->IDENTIFIER()->getText();
  auto initializer = std::move(valueStack.top());
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
  if (currTypeNode != nullptr && currTypeNode->isLeaf()) {
    auto targetType = types::Type(std::move(currTypeNode));
    initializer = castRegister(initializer, targetType);
  }

  const auto scopedIdentifier = identifier + symbolTable.getCurrentScopeID();
  LLVMGenerator::Type type;
  if (initializer.type.isLeaf()) {
    const auto& rootType = initializer.type.getRoot();
    int typeID = *rootType.getSimpleTypeId();
    if (typeID == INT_TYPE_ID) {
      type = LLVMGenerator::Type::I32;
    }
    if (typeID == DOUBLE_TYPE_ID) {
      type = LLVMGenerator::Type::DOUBLE;
    }
    if (typeID == BOOL_TYPE_ID) {
      initializer.name = generator.extI1toI8(initializer.name);
      type = LLVMGenerator::Type::I8;
    }
    if (typeID == FLOAT_TYPE_ID) {
      type = LLVMGenerator::Type::FLOAT;
    }
  }

  generator.createDeclaration(type, scopedIdentifier);
  generator.createAssignment(type, scopedIdentifier, initializer.name);
  symbolTable.insertInCurrentScope(std::make_pair(
      identifier,
      Value(identifier, std::move(initializer.type), Value::Category::MEMORY)));
}

void TranslatorListener::enterCompoundStatement(
    Lexy2Parser::CompoundStatementContext* ctx) {
  // note that this won't be skipped in panic mode
  symbolTable.enterNewScope();
}

void TranslatorListener::exitCompoundStatement(
    Lexy2Parser::CompoundStatementContext* ctx) {
  // note that this won't be skipped in panic mode
  symbolTable.leaveScope();
}

void TranslatorListener::exitCondition(Lexy2Parser::ConditionContext* ctx) {
  if (inErrorMode)
    return;

  Value cond = std::move(valueStack.top());
  valueStack.pop();
  if (cond.type.getRoot().getSimpleTypeId() != BOOL_TYPE_ID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Condition must be of boolean type");
    inErrorMode = true;
    return;
  }
  auto [endOrElseLabel, thenLabel] = utils::peekTwo(basicBlockStack);
  generator.createBranch(cond.name, thenLabel, endOrElseLabel);
}

void TranslatorListener::enterThenPart(Lexy2Parser::ThenPartContext* ctx) {
  auto thenLabel = basicBlockStack.top();
  basicBlockStack.pop();
  generator.createLabel(thenLabel);
}

void TranslatorListener::exitThenPart(Lexy2Parser::ThenPartContext* ctx) {
  if (inErrorMode)
    return;

  generator.createBranch(returnPointsStack.top());
}

void TranslatorListener::enterElsePart(Lexy2Parser::ElsePartContext* ctx) {
  if (inErrorMode)
    return;

  auto elseLabel = basicBlockStack.top();
  basicBlockStack.pop();
  generator.createLabel(elseLabel);
}

void TranslatorListener::exitElsePart(Lexy2Parser::ElsePartContext* ctx) {
  if (inErrorMode)
    return;

  generator.createBranch(returnPointsStack.top());
}

void TranslatorListener::enterIf(Lexy2Parser::IfContext* ctx) {
  if (inErrorMode)
    return;

  const auto endLabel = generator.getIfEndLabel();
  basicBlockStack.push(endLabel);
  basicBlockStack.push(generator.getIfThenLabel());
  returnPointsStack.push(endLabel);
}

void TranslatorListener::exitIf(Lexy2Parser::IfContext* ctx) {
  if (inErrorMode)
    return;

  auto endLabel = basicBlockStack.top();
  basicBlockStack.pop();
  generator.createLabel(endLabel);
  returnPointsStack.pop();
}

void TranslatorListener::enterIfElse(Lexy2Parser::IfElseContext* ctx) {
  if (inErrorMode)
    return;

  const auto endLabel = generator.getIfEndLabel();
  basicBlockStack.push(endLabel);
  basicBlockStack.push(generator.getIfElseLabel());
  basicBlockStack.push(generator.getIfThenLabel());
  returnPointsStack.push(endLabel);
}

void TranslatorListener::exitIfElse(Lexy2Parser::IfElseContext* ctx) {
  if (inErrorMode)
    return;

  auto endLabel = basicBlockStack.top();
  basicBlockStack.pop();
  generator.createLabel(endLabel);
  returnPointsStack.pop();
}

void TranslatorListener::enterWhileLoopBody(
    Lexy2Parser::WhileLoopBodyContext* ctx) {
  if (inErrorMode)
    return;

  auto bodyLabel = basicBlockStack.top();
  basicBlockStack.pop();
  generator.createLabel(bodyLabel);
}

void TranslatorListener::exitWhileLoopBody(
    Lexy2Parser::WhileLoopBodyContext* ctx) {
  if (inErrorMode)
    return;

  auto condLabel = returnPointsStack.top();
  returnPointsStack.pop();
  generator.createBranch(condLabel);
}

void TranslatorListener::enterWhileLoopCondition(
    Lexy2Parser::WhileLoopConditionContext* ctx) {
  if (inErrorMode)
    return;

  auto condLabel = basicBlockStack.top();
  basicBlockStack.pop();
  generator.createBranch(condLabel);
  generator.createLabel(condLabel);
}

void TranslatorListener::exitWhileLoopCondition(
    Lexy2Parser::WhileLoopConditionContext* ctx) {
  if (inErrorMode)
    return;

  Value cond = std::move(valueStack.top());
  valueStack.pop();
  if (cond.type.getRoot().getSimpleTypeId() != BOOL_TYPE_ID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Condition must be of boolean type");
    inErrorMode = true;
    return;
  }
  auto [endLabel, bodyLabel] = utils::peekTwo(basicBlockStack);
  generator.createBranch(cond.name, bodyLabel, endLabel);
}

void TranslatorListener::enterWhileLoop(Lexy2Parser::WhileLoopContext* ctx) {
  if (inErrorMode)
    return;

  basicBlockStack.push(generator.getWhileEndLabel());
  basicBlockStack.push(generator.getWhileBodyLabel());
  auto condLabel = generator.getWhileCondLabel();
  basicBlockStack.push(condLabel);
  returnPointsStack.push(condLabel);
}

void TranslatorListener::exitWhileLoop(Lexy2Parser::WhileLoopContext* ctx) {
  if (inErrorMode)
    return;

  auto end = basicBlockStack.top();
  basicBlockStack.pop();
  generator.createLabel(end);
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
  auto value = std::move(valueStack.top());
  valueStack.pop();
  if (value.category == Value::Category::MEMORY) {
    value = load(value);
  }
  auto variable = iter->second;
  variable.name += symbolTable.getScopeID(depth);

  if (variable.type.getSimpleTypeId() != value.type.getSimpleTypeId()) {
    if (typeManager.isImplicitFromTo(value.type, variable.type)) {
      value = castRegister(value, variable.type);
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
  if (variable.type.isLeaf()) {
    int variableTypeID = *variable.type.getSimpleTypeId();
    if (variableTypeID == DOUBLE_TYPE_ID) {
      generator.createAssignment(LLVMGenerator::Type::DOUBLE, scopedIdentifier,
                                 value.name);
    }
    if (variableTypeID == INT_TYPE_ID) {
      generator.createAssignment(LLVMGenerator::Type::I32, scopedIdentifier,
                                 value.name);
    }
    if (variableTypeID == BOOL_TYPE_ID) {
      value.name = generator.extI1toI8(value.name);
      generator.createAssignment(LLVMGenerator::Type::I8, scopedIdentifier,
                                 value.name);
    }
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

  if (left.type.getSimpleTypeId() != right.type.getSimpleTypeId()) {
    if (!applyBuiltInConversions(left, right, ctx)) {
      inErrorMode = true;
      return;
    }
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

  if (left.type.getSimpleTypeId() != right.type.getSimpleTypeId()) {
    if (!applyBuiltInConversions(left, right, ctx)) {
      inErrorMode = true;
      return;
    }
  }

  auto op = ctx->op->getText();
  if (op == "<") {
    if (!typeManager.isOperatorSupported(Operator::LT, left.type)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '<' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(compareRegisters(left, right, Relation::LT));
  }
  if (op == ">") {
    if (!typeManager.isOperatorSupported(Operator::GT, left.type)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '>' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(compareRegisters(left, right, Relation::GT));
  }
  if (op == "<=") {
    if (!typeManager.isOperatorSupported(Operator::LE, left.type)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '<=' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(compareRegisters(left, right, Relation::LE));
  }
  if (op == ">=") {
    if (!typeManager.isOperatorSupported(Operator::GE, left.type)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '>=' cannot be applied to arguments of type ? and ?");
      return;
    }
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

  if (left.type.getSimpleTypeId() != right.type.getSimpleTypeId()) {
    if (!applyBuiltInConversions(left, right, ctx)) {
      inErrorMode = true;
      return;
    }
  }

  auto op = ctx->op->getText();
  if (op == "+") {
    if (!typeManager.isOperatorSupported(Operator::ADD, left.type)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '+' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(addRegisters(left, right));
  }
  if (op == "-") {
    if (!typeManager.isOperatorSupported(Operator::SUB, left.type)) {
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

  if (left.type.getSimpleTypeId() != right.type.getSimpleTypeId()) {
    if (!applyBuiltInConversions(left, right, ctx)) {
      inErrorMode = true;
      return;
    }
  }

  auto op = ctx->op->getText();
  if (op == "*") {
    if (!typeManager.isOperatorSupported(Operator::MUL, left.type)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '*' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(multiplyRegisters(left, right));
  }
  if (op == "/") {
    if (!typeManager.isOperatorSupported(Operator::DIV, left.type)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Operator '/' cannot be applied to arguments of type ? and ?");
      return;
    }
    valueStack.push(divideRegisters(left, right));
  }
  if (op == "%") {
    if (!typeManager.isOperatorSupported(Operator::REM, left.type)) {
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
  const auto targetTypeStr = ctx->TYPE_ID()->getText();
  auto targetType = types::Type(std::make_unique<types::LeafNode>(
      static_cast<PrimitiveType>(typeIDs[targetTypeStr])));
  valueStack.push(castRegister(value, targetType));
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
    if (!typeManager.isOperatorSupported(Operator::NEG, value.type)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Unary operator '-' cannot be applied to argument of type ?");
      return;
    }
    valueStack.push(negateRegister(value));
  }
  if (ctx->op->getText() == "+") {
    if (!typeManager.isOperatorSupported(Operator::POS, value.type)) {
      inErrorMode = true;
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Unary operator '+' cannot be applied to argument of type ?");
      return;
    }
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

  valueStack.push(
      Value(ctx->INTEGER_LITERAL()->getText(),
            types::Type(std::make_unique<types::LeafNode>(PrimitiveType::INT)),
            Value::Category::CONSTANT));
}

void TranslatorListener::exitFloatLiteral(
    Lexy2Parser::FloatLiteralContext* ctx) {
  if (inErrorMode)
    return;

  valueStack.push(Value(
      ctx->FLOAT_LITERAL()->getText(),
      types::Type(std::make_unique<types::LeafNode>(PrimitiveType::DOUBLE)),
      Value::Category::CONSTANT));
}

void TranslatorListener::exitBoolLiteral(Lexy2Parser::BoolLiteralContext* ctx) {
  if (inErrorMode)
    return;

  valueStack.push(
      Value(ctx->BOOL_LITERAL()->getText(),
            types::Type(std::make_unique<types::LeafNode>(PrimitiveType::BOOL)),
            Value::Category::CONSTANT));
}

void TranslatorListener::exitRankSpecifier(
    Lexy2Parser::RankSpecifierContext* ctx) {
  int dim = std::stoi(ctx->INTEGER_LITERAL()->getText());
  currTypeNode =
      std::make_unique<types::ArrayNode>(dim, std::move(currTypeNode));
}

void TranslatorListener::exitSimpleType(Lexy2Parser::SimpleTypeContext* ctx) {
  auto typeID = typeIDs[ctx->TYPE_ID()->getText()];
  currTypeNode =
      std::make_unique<types::LeafNode>(static_cast<PrimitiveType>(typeID));
}

std::string TranslatorListener::getCode(const std::string& sourceFilename) {
  return generator.emitCode(sourceFilename);
}
}  // namespace lexy2