#include "translator_listener.hpp"
#include <algorithm>
#include <tuple>
#include "types/llvm_str_visitor.hpp"
#include "types/type.hpp"
#include "utils.hpp"

namespace lexy2 {
TranslatorListener::TranslatorListener(ErrorHandler& errorHandler)
    : errorHandler(errorHandler), typeNodeFactory(typeManager) {}

void TranslatorListener::exitStatement(Lexy2Parser::StatementContext* ctx) {
  if (inErrorMode) {  // synchronize
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
  if (value.isInMemory()) {
    value = load(value);
  }
  if (value.type.isLeaf()) {
    int typeID = *value.type.getSimpleTypeId();
    if (!typeManager.isPrimitiveType(typeID)) {
      errorHandler.reportError(utils::getLineCol(ctx),
                               "print can only be used with built-in types");
    }
    if (typeID == typeManager.INT_TYPE_ID) {
      generator.printI32(value.name);
    }
    if (typeID == typeManager.DOUBLE_TYPE_ID) {
      generator.printDouble(value.name);
    }
    if (typeID == typeManager.BOOL_TYPE_ID) {
      const auto cast = generator.extBoolToI32(value.name);
      generator.printI32(cast);
    }
    if (typeID == typeManager.FLOAT_TYPE_ID) {
      auto cast = generator.extFloatToDouble(value.name);
      generator.printDouble(cast);
    }
  }
}

void TranslatorListener::exitReadIntrinsic(
    Lexy2Parser::ReadIntrinsicContext* ctx) {
  if (inErrorMode)
    return;

  auto value = std::move(valueStack.top());
  valueStack.pop();
  if (!value.isInMemory()) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Can't read into an rvalue");
    inErrorMode = true;
    return;
  }
  if (value.type.isLeaf()) {
    int typeID = *value.type.getSimpleTypeId();
    if (!typeManager.isPrimitiveType(typeID)) {
      errorHandler.reportError(utils::getLineCol(ctx),
                               "read can only be used with built-in types");
      inErrorMode = true;
      return;
    }
    generator.read(value.name, value.type,
                   value.category == Value::Category::INTERNAL_PTR);
  }
}

bool areSameType(const types::Type& t1, const types::Type& t2) {
  if (!t1.getSimpleTypeId().has_value() || !t2.getSimpleTypeId().has_value()) {
    throw std::invalid_argument(
        "Conversions between non-simple types not implemented");
  }

  return *t1.getSimpleTypeId() == *t2.getSimpleTypeId();
}

Error<std::string> TranslatorListener::declareVariable(
    const std::string& identifier, const Value& initializer,
    const std::optional<types::Type> declaredType) {
  Value loadedInitializer = initializer;
  if (initializer.isInMemory()) {
    loadedInitializer = load(initializer);
  }
  if (symbolTable.currentScopeFind(identifier) !=
      symbolTable.getCurrentScope().end()) {
    return error("Identifier '" + identifier + "' already in use");
  }

  if (declaredType.has_value()) {
    if (declaredType->isLeaf() &&
        !dynamic_cast<const types::LeafNode&>(declaredType->getRoot())
             .isUserDefined()) {
      if (!areSameType(loadedInitializer.type, *declaredType)) {
        loadedInitializer = castRegister(loadedInitializer, *declaredType);
      }
    } else {
      loadedInitializer.type = *declaredType;
    }
  }

  const auto scopedIdentifier = identifier + symbolTable.getCurrentScopeID();

  if (loadedInitializer.type.isLeaf() &&
      loadedInitializer.type.getSimpleTypeId() == typeManager.BOOL_TYPE_ID) {
    loadedInitializer.name = generator.extI1toI8(loadedInitializer.name);
  }

  const auto& initializerType = loadedInitializer.type;

  generator.createDeclaration(initializerType, scopedIdentifier, false);
  if (initializerType.isLeaf() &&
      !dynamic_cast<const types::LeafNode&>(initializerType.getRoot())
           .isUserDefined()) {
    generator.createAssignment(initializerType, scopedIdentifier,
                               loadedInitializer.name, false);
  }

  symbolTable.insertInCurrentScope(std::make_pair(
      identifier,
      Value(identifier, loadedInitializer.type, Value::Category::MEMORY)));

  return {};
}

void TranslatorListener::exitVariableDeclaration(
    Lexy2Parser::VariableDeclarationContext* ctx) {
  if (inErrorMode)
    return;

  auto identifier = ctx->IDENTIFIER()->getText();
  if (typeManager.getTypeID(identifier)) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Type name not allowed in this context");
    inErrorMode = true;
    return;
  }
  if (valueStack.empty()) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "No initializer for '" + identifier + "'");
    inErrorMode = true;
    return;
  }
  auto initializer = std::move(valueStack.top());
  valueStack.pop();

  std::optional<types::Type> declaredType;
  if (currTypeNode != nullptr) {
    declaredType = types::Type(std::move(currTypeNode));
  } else {
    declaredType = {};
  }
  auto err = declareVariable(identifier, initializer, declaredType);
  if (err) {
    errorHandler.reportError(utils::getLineCol(ctx), err.message());
    inErrorMode = true;
    return;
  }
}

void TranslatorListener::enterFunctionDeclaration(
    Lexy2Parser::FunctionDeclarationContext* ctx) {
  functionParams.push({});
  generator.enterFunction();
}

Error<std::string> TranslatorListener::declareFunction(
    const std::string& functionName, const types::Type& retType,
    const std::vector<FunctionParam>& functionParams) {
  if (symbolTable.currentScopeFind(functionName) !=
      symbolTable.getCurrentScope().end()) {
    return error("Identifier '" + functionName + "' already in use");
  }

  if (!retType.isLeaf()) {
    throw std::runtime_error("Not implemented");
  }

  generator.createFunction(functionName + symbolTable.getCurrentScopeID(),
                           functionParams, retType);

  std::vector<std::unique_ptr<types::TypeNode>> typeNodes;
  for (const auto& param : functionParams) {
    typeNodes.push_back(types::cloneNode(param.type.getRoot()));
  }
  types::Type functionType(std::make_unique<types::FunctionNode>(
      std::move(typeNodes), types::cloneNode(retType.getRoot())));
  symbolTable.insertInCurrentScope(std::make_pair(
      functionName,
      Value(functionName, functionType, Value::Category::MEMORY)));

  return {};
}

void TranslatorListener::exitFunctionDeclaration(
    Lexy2Parser::FunctionDeclarationContext* ctx) {
  if (inErrorMode)
    return;

  auto err = declareFunction(functionNames.top(), retTypesStack.top(),
                             functionParams.top());
  if (err) {
    errorHandler.reportError(utils::getLineCol(ctx), err.message());
    inErrorMode = true;
    return;
  }

  functionParams.pop();
  retTypesStack.pop();
  functionNames.pop();
  generator.exitFunction();
}

void TranslatorListener::exitStructDeclaration(
    Lexy2Parser::StructDeclarationContext* ctx) {
  auto structName = ctx->IDENTIFIER()->getText();
  if (!typeManager.addType(structName)) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Type name '" + structName + "' already declared");
    inErrorMode = true;
    return;
  }
  auto typeID = *typeManager.getTypeID(structName);
  Struct struct_;
  struct_.name = std::move(structName);

  int fieldsCnt = fieldNames.size();
  for (int i = 0; i < fieldsCnt; ++i) {
    const auto& fieldName = fieldNames[i];
    const auto& fieldType = fieldTypes[i];
    auto it = std::find_if(
        struct_.fields.begin(), struct_.fields.end(),
        [&fieldName](const std::pair<std::string, types::Type>& x) {
          return x.first == fieldName;
        });
    if (it != struct_.fields.end()) {
      errorHandler.reportError(utils::getLineCol(ctx),
                               "Redefinition of field '" + fieldName +
                                   "' in struct '" + struct_.name);
      inErrorMode = true;
      return;
    }
    struct_.fields.emplace_back(std::move(fieldName), std::move(fieldType));
  }

  generator.createStruct(struct_);
  typeManager.addStruct(typeID, std::move(struct_));

  fieldNames.clear();
  fieldTypes.clear();
}

void TranslatorListener::exitStructField(Lexy2Parser::StructFieldContext* ctx) {
  auto fieldName = ctx->IDENTIFIER()->getText();

  fieldNames.emplace_back(std::move(fieldName));
  fieldTypes.emplace_back(std::move(currTypeNode));
}

void TranslatorListener::exitFunctionName(
    Lexy2Parser::FunctionNameContext* ctx) {
  auto functionName = ctx->IDENTIFIER()->getText();
  functionNames.push(functionName);
  if (typeManager.getTypeID(functionName)) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Type name not allowed in this context");
    inErrorMode = true;
    return;
  }
}

void TranslatorListener::exitReturnType(Lexy2Parser::ReturnTypeContext* ctx) {
  retTypesStack.push(types::Type(std::move(currTypeNode)));
}

void TranslatorListener::exitParam(Lexy2Parser::ParamContext* ctx) {
  auto paramName = ctx->IDENTIFIER()->getText();
  if (typeManager.getTypeID(paramName)) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Type name not allowed in this context");
    inErrorMode = true;
    return;
  }
  functionParams.top().push_back(
      FunctionParam(paramName, types::Type(std::move(currTypeNode))));
}

void TranslatorListener::enterFunctionBody(
    Lexy2Parser::FunctionBodyContext* ctx) {
  symbolTable.enterNewScope();
  // allocate memory for arguments
  for (const auto& param : functionParams.top()) {
    const auto& paramType = param.type;
    if (!paramType.isLeaf()) {
      throw std::runtime_error("Not implemented");
    }
    auto typeID = *paramType.getSimpleTypeId();
    const auto scopedIdentifier = param.name + symbolTable.getCurrentScopeID();
    if (typeID == typeManager.BOOL_TYPE_ID) {
      auto extParam = generator.extI1toI8("%" + param.name);
      generator.createDeclaration(paramType, scopedIdentifier, false);
      generator.createAssignment(paramType, scopedIdentifier, extParam, false);
    } else {
      generator.createDeclaration(paramType, scopedIdentifier, false);
      generator.createAssignment(paramType, scopedIdentifier, "%" + param.name,
                                 false);
    }

    symbolTable.insertInCurrentScope(std::make_pair(
        param.name, Value(param.name, param.type, Value::Category::MEMORY)));
  }

  // allocate memory for return variable
  auto typeID = *retTypesStack.top().getSimpleTypeId();
  generator.createDeclaration(retTypesStack.top(), "retVal", false);
}

void TranslatorListener::exitFunctionBody(
    Lexy2Parser::FunctionBodyContext* ctx) {
  symbolTable.leaveScope();
  auto loaded = generator.createLoad(retTypesStack.top(), "retVal", true);
  generator.createReturn(retTypesStack.top(), loaded);
}

void TranslatorListener::exitReturnStatement(
    Lexy2Parser::ReturnStatementContext* ctx) {
  if (inErrorMode)
    return;

  auto value = valueStack.top();
  valueStack.pop();
  if (value.isInMemory()) {
    value = load(value);
  }

  if (value.type.getSimpleTypeId() != retTypesStack.top().getSimpleTypeId()) {
    if (typeManager.isImplicitFromTo(value.type, retTypesStack.top())) {
      value = castRegister(value, retTypesStack.top());
    } else {
      errorHandler.reportError(utils::getLineCol(ctx),
                               "No implicit conversion from ? to ?");
      inErrorMode = true;
      return;
    }
  }

  auto typeID = *value.type.getSimpleTypeId();
  if (typeID == typeManager.BOOL_TYPE_ID) {
    value.name = generator.extI1toI8(value.name);
  }

  generator.createAssignment(value.type, "retVal", value.name, false);
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
  if (cond.type.getRoot().getSimpleTypeId() != typeManager.BOOL_TYPE_ID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Condition must be of boolean type");
    inErrorMode = true;
    return;
  }
  if (cond.isInMemory()) {
    cond = load(cond);
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
  if (cond.type.getRoot().getSimpleTypeId() != typeManager.BOOL_TYPE_ID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Condition must be of boolean type");
    inErrorMode = true;
    return;
  }
  if (cond.isInMemory()) {
    cond = load(cond);
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

  auto value = std::move(valueStack.top());
  valueStack.pop();

  auto lhs = std::move(valueStack.top());
  valueStack.pop();
  if (lhs.category != Value::Category::MEMORY &&
      lhs.category != Value::Category::INTERNAL_PTR) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "r-value cannot be assigned to");
    inErrorMode = true;
    return;
  }

  if (lhs.category == Value::Category::MEMORY) {
    assignToVariable(lhs, value, ctx);
  }
  if (lhs.category == Value::Category::INTERNAL_PTR) {
    assignToInternalPtr(lhs, value, ctx);
  }
}

void TranslatorListener::exitLogicalAnd(Lexy2Parser::LogicalAndContext* ctx) {
  if (inErrorMode)
    return;

  auto [left, right] = utils::popTwo(valueStack);
  if (left.type.getSimpleTypeId() != typeManager.BOOL_TYPE_ID ||
      right.type.getSimpleTypeId() != typeManager.BOOL_TYPE_ID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Both operands of '&' must be of boolean type");
    inErrorMode = true;
    return;
  }

  if (left.isInMemory()) {
    left = load(left);
  }
  if (right.isInMemory()) {
    right = load(right);
  }

  auto result = generator.createBinOp(
      LLVMGenerator::BinOpName::AND,
      types::Type(typeNodeFactory.createLeafNode(TypeManager::BOOL_TYPE_ID)),
      left.name, right.name);

  valueStack.push(Value(result, types::Type(typeNodeFactory.createLeafNode(
                                    TypeManager::BOOL_TYPE_ID))));
}

void TranslatorListener::exitLogicalOr(Lexy2Parser::LogicalOrContext* ctx) {
  if (inErrorMode)
    return;

  auto [left, right] = utils::popTwo(valueStack);
  if (left.type.getSimpleTypeId() != typeManager.BOOL_TYPE_ID ||
      right.type.getSimpleTypeId() != typeManager.BOOL_TYPE_ID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Both operands of '|' must be of boolean type");
    inErrorMode = true;
    return;
  }

  if (left.isInMemory()) {
    left = load(left);
  }
  if (right.isInMemory()) {
    right = load(right);
  }

  auto result = generator.createBinOp(
      LLVMGenerator::BinOpName::OR,
      types::Type(typeNodeFactory.createLeafNode(TypeManager::BOOL_TYPE_ID)),
      left.name, right.name);

  valueStack.push(Value(result, types::Type(typeNodeFactory.createLeafNode(
                                    TypeManager::BOOL_TYPE_ID))));
}

void TranslatorListener::exitLogicalXor(Lexy2Parser::LogicalXorContext* ctx) {
  if (inErrorMode)
    return;

  auto [left, right] = utils::popTwo(valueStack);
  if (left.type.getSimpleTypeId() != typeManager.BOOL_TYPE_ID ||
      right.type.getSimpleTypeId() != typeManager.BOOL_TYPE_ID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Both operands of '^' must be of boolean type");
    inErrorMode = true;
    return;
  }

  if (left.isInMemory()) {
    left = load(left);
  }
  if (right.isInMemory()) {
    right = load(right);
  }

  auto result = generator.createBinOp(
      LLVMGenerator::BinOpName::XOR,
      types::Type(typeNodeFactory.createLeafNode(TypeManager::BOOL_TYPE_ID)),
      left.name, right.name);

  valueStack.push(Value(result, types::Type(typeNodeFactory.createLeafNode(
                                    TypeManager::BOOL_TYPE_ID))));
}

void TranslatorListener::exitEquality(Lexy2Parser::EqualityContext* ctx) {
  if (inErrorMode)
    return;

  auto [left, right] = utils::popTwo(valueStack);
  if (left.isInMemory()) {
    left = load(left);
  }
  if (right.isInMemory()) {
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
  if (left.isInMemory()) {
    left = load(left);
  }
  if (right.isInMemory()) {
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
  if (left.isInMemory()) {
    left = load(left);
  }
  if (right.isInMemory()) {
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
  if (left.isInMemory()) {
    left = load(left);
  }
  if (right.isInMemory()) {
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
  if (value.isInMemory()) {
    value = load(value);
  }
  const auto targetTypeStr = ctx->IDENTIFIER()->getText();
  auto targetTypeID = typeManager.getTypeID(targetTypeStr);
  if (!targetTypeID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "'" + targetTypeStr + "' does not name a type");
    inErrorMode = true;
    return;
  }
  auto targetType = types::Type(typeNodeFactory.createLeafNode(*targetTypeID));
  if (!areSameType(value.type, targetType))
    valueStack.push(castRegister(value, targetType));
}

void TranslatorListener::exitUnary(Lexy2Parser::UnaryContext* ctx) {
  if (inErrorMode)
    return;

  auto value = valueStack.top();
  valueStack.pop();
  if (value.isInMemory()) {
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
  if (ctx->op->getText() == "!") {
    if (value.type.getSimpleTypeId() != typeManager.BOOL_TYPE_ID) {
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Unary operator '!' can only be applied to boolean argument");
      inErrorMode = true;
      return;
    }

    auto result = generator.createBinOp(LLVMGenerator::BinOpName::XOR,
                                        value.type, value.name, "true");
    valueStack.push(Value(result, value.type));
  }
}

void TranslatorListener::exitIdentifier(Lexy2Parser::IdentifierContext* ctx) {
  if (inErrorMode)
    return;

  const auto id = ctx->IDENTIFIER()->getText();
  if (typeManager.getTypeID(id)) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Type name not allowed in this context");
    inErrorMode = true;
    return;
  }
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

  valueStack.push(Value(
      ctx->INTEGER_LITERAL()->getText(),
      types::Type(typeNodeFactory.createLeafNode(TypeManager::INT_TYPE_ID)),
      Value::Category::CONSTANT));
}

void TranslatorListener::exitFloatLiteral(
    Lexy2Parser::FloatLiteralContext* ctx) {
  if (inErrorMode)
    return;

  valueStack.push(Value(
      ctx->FLOAT_LITERAL()->getText(),
      types::Type(typeNodeFactory.createLeafNode(TypeManager::DOUBLE_TYPE_ID)),
      Value::Category::CONSTANT));
}

void TranslatorListener::exitBoolLiteral(Lexy2Parser::BoolLiteralContext* ctx) {
  if (inErrorMode)
    return;

  valueStack.push(Value(
      ctx->BOOL_LITERAL()->getText(),
      types::Type(typeNodeFactory.createLeafNode(TypeManager::BOOL_TYPE_ID)),
      Value::Category::CONSTANT));
}

void TranslatorListener::exitElementIndex(
    Lexy2Parser::ElementIndexContext* ctx) {
  if (inErrorMode)
    return;

  auto [arr, idx] = utils::popTwo(valueStack);
  if (idx.type.getSimpleTypeId() != typeManager.INT_TYPE_ID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Only int allowed as the type of the indexer");
    inErrorMode = true;
    return;
  }
  if (idx.isInMemory()) {
    idx = load(idx);
    idx.name = generator.extI32toI64(idx.name);
  }

  auto typeStr = arr.type.getLLVMString(false);

  auto arrayIdx = generator.getElementPtrInBounds(
      arr.name, idx.name, typeStr,
      arr.category == Value::Category::INTERNAL_PTR);
  auto peeledType = arr.type.getPeeledType();
  if (!peeledType.has_value()) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "Can't apply access operator to simple type");
    inErrorMode = true;
    return;
  }

  valueStack.push(Value(arrayIdx, *arr.type.getPeeledType(),
                        Value::Category::INTERNAL_PTR));
}

void TranslatorListener::enterFunctionCall(
    Lexy2Parser::FunctionCallContext* ctx) {
  functionArgsCount = 0;
}

void TranslatorListener::exitFunctionCall(
    Lexy2Parser::FunctionCallContext* ctx) {
  if (inErrorMode)
    return;

  std::vector<FunctionParam> args;

  for (int i = 0; i < functionArgsCount; ++i) {
    auto value = std::move(valueStack.top());
    valueStack.pop();
    if (value.isInMemory()) {
      value = load(value);
    }
    args.push_back(FunctionParam(value.name, value.type));
  }
  std::reverse(args.begin(), args.end());

  auto function = std::move(valueStack.top());
  valueStack.pop();
  const auto& functionNode =
      dynamic_cast<const types::FunctionNode&>(function.type.getRoot());
  const auto& paramTypes = functionNode.getParamTypes();

  if (functionArgsCount != paramTypes.size()) {
    errorHandler.reportError(
        utils::getLineCol(ctx),
        "Function '" + function.name + "' accepts " +
            std::to_string(paramTypes.size()) + " arguments but " +
            std::to_string(functionArgsCount) + " were provided");
    inErrorMode = true;
    return;
  }
  for (int i = 0; i < functionArgsCount; ++i) {
    if (paramTypes[i]->getSimpleTypeId() != args[i].type.getSimpleTypeId()) {
      errorHandler.reportError(
          utils::getLineCol(ctx),
          "Type mismatch in call to '" + function.name + "'");
      inErrorMode = true;
      return;
    }
  }

  auto typeID = *functionNode.getReturnType()->getSimpleTypeId();
  auto retType = types::Type(types::cloneNode(*functionNode.getReturnType()));
  auto callResult = generator.createCall(function.name, args, retType);
  valueStack.push(Value(callResult, retType));
}

void TranslatorListener::exitFunctionArg(Lexy2Parser::FunctionArgContext* ctx) {
  if (inErrorMode)
    return;

  ++functionArgsCount;
}

void TranslatorListener::exitArrayType(Lexy2Parser::ArrayTypeContext* ctx) {
  while (!rankSpecStack.empty()) {
    int rank = rankSpecStack.top();
    rankSpecStack.pop();
    currTypeNode =
        std::make_unique<types::ArrayNode>(rank, std::move(currTypeNode));
  }
}

void TranslatorListener::exitRankSpecifier(
    Lexy2Parser::RankSpecifierContext* ctx) {
  int dim = std::stoi(ctx->INTEGER_LITERAL()->getText());

  rankSpecStack.push(dim);
}

void TranslatorListener::exitSimpleType(Lexy2Parser::SimpleTypeContext* ctx) {
  auto typeStr = ctx->IDENTIFIER()->getText();
  auto typeID = typeManager.getTypeID(ctx->IDENTIFIER()->getText());
  if (!typeID) {
    errorHandler.reportError(utils::getLineCol(ctx),
                             "'" + typeStr + "' does not name a type");
    inErrorMode = true;
    return;
  }
  currTypeNode = typeNodeFactory.createLeafNode(*typeID);
}

std::string TranslatorListener::getCode(const std::string& sourceFilename) {
  return generator.emitCode(sourceFilename);
}
}  // namespace lexy2