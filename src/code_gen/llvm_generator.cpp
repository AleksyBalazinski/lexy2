#include "code_gen/llvm_generator.hpp"
#include <sstream>
#include "code_gen/topo_sorter.hpp"
#include "types/llvm_str_visitor.hpp"
#include "utils.hpp"

namespace lexy2 {
std::string LLVMGenerator::getOpPrefix(const types::Type& type, BinOpName op) {
  if (op != BinOpName::DIV && op != BinOpName::REM) {
    if (type.isInteral()) {
      return "";
    }
    if (type.isFloatingPoint()) {
      return "f";
    }
    return "";
  } else {
    if (type.isInteral()) {
      return "s";
    }
    if (type.isFloatingPoint()) {
      return "f";
    }
    return "";
  }
}

std::string LLVMGenerator::getRelPrefix(const types::Type& type) {
  if (type.isInteral()) {
    return "i";
  }
  if (type.isFloatingPoint()) {
    return "f";
  }
  return "";
}

std::string LLVMGenerator::getOperationString(BinOpName op) {
  switch (op) {
    case BinOpName::SUB:
      return "sub";
    case BinOpName::ADD:
      return "add";
    case BinOpName::MUL:
      return "mul";
    case BinOpName::DIV:
      return "div";
    case BinOpName::REM:
      return "rem";
    case BinOpName::CMP:
      return "cmp";
    case BinOpName::AND:
      return "and";
    case BinOpName::OR:
      return "or";
    case BinOpName::XOR:
      return "xor";
    default:
      return "";
  }
}

std::string LLVMGenerator::getRelName(Relation Relation) {
  switch (Relation) {
    case Relation::EQ:
      return "eq";
    case Relation::NEQ:
      return "ne";
    case Relation::GE:
      return "ge";
    case Relation::GT:
      return "gt";
    case Relation::LE:
      return "le";
    case Relation::LT:
      return "lt";
    default:
      return "";
  }
}

std::string LLVMGenerator::getRelNamePrefix(Relation Relation,
                                            const types::Type& type) {
  if (type.isFloatingPoint()) {
    return "o";
  }
  if (type.isInteral()) {
    if (Relation == Relation::EQ || Relation == Relation::NEQ)
      return "";
    else
      return "s";
  }
  throw std::runtime_error("Not implemented");
}

std::string LLVMGenerator::getIndent() {
  return "  ";
}

LLVMGenerator::LLVMGenerator() {
  formatStrs = {formatIntNewLine, formatFloatNewLine, formatInt, formatDouble,
                formatFloat};
}

std::string LLVMGenerator::createBinOp(BinOpName op, const types::Type& type,
                                       const std::string& arg1,
                                       const std::string& arg2) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = " + getOpPrefix(type, op) +
               getOperationString(op) + " " + type.getLLVMString(true) + " " +
               arg1 + ", " + arg2 + "\n";
  ++reg;
  return regStr;
}
std::string LLVMGenerator::createRel(const types::Type& type, Relation Relation,
                                     const std::string& arg1,
                                     const std::string& arg2) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = " + getRelPrefix(type) + "cmp" + " " +
               getRelNamePrefix(Relation, type) + getRelName(Relation) + " " +
               type.getLLVMString(true) + " " + arg1 + ", " + arg2 + "\n";
  ++reg;
  return regStr;
}

void LLVMGenerator::createAssignment(const types::Type& type,
                                     const std::string& identifier,
                                     const std::string& value, bool boolAsI1,
                                     bool isInternalPtr) {
  std::string idString = (isInternalPtr ? "" : "%") + identifier;
  getText() += getIndent() + "store " + type.getLLVMString(boolAsI1) + " " +
               value + ", " + type.getLLVMString(boolAsI1) + "* " + idString +
               "\n";
}

void LLVMGenerator::createDeclaration(const types::Type& type,
                                      const std::string& arg, bool boolAsI1) {
  getText() += getIndent() + "%" + arg + " = alloca " +
               type.getLLVMString(boolAsI1) + "\n";
}

std::string LLVMGenerator::createLoad(const types::Type& type,
                                      const std::string& id, bool boolAsI1,
                                      bool isInternalPtr) {
  const auto regStr = getRegStr();
  std::string idString = (isInternalPtr ? "" : "%") + id;
  getText() += getIndent() + regStr + " = load " +
               type.getLLVMString(boolAsI1) + ", " +
               type.getLLVMString(boolAsI1) + "* " + idString + "\n";
  ++reg;
  return regStr;
}

void LLVMGenerator::createBranch(const std::string& cond,
                                 const std::string& ifTrue,
                                 const std::string& ifFalse) {
  getText() += getIndent() + "br i1 " + cond + ", label %" + ifTrue +
               ", label %" + ifFalse + "\n";
}

void LLVMGenerator::createBranch(const std::string& dest) {
  getText() += getIndent() + "br label %" + dest + "\n";
}

void LLVMGenerator::createLabel(const std::string& label) {
  getText() += label + ":\n";
}

void LLVMGenerator::createFunction(const std::string& functionName,
                                   const std::vector<FunctionParam>& params,
                                   const types::Type& retType) {
  const char* sep = "";
  std::string paramsStr;
  for (const auto& param : params) {
    const auto& name = param.name;
    const auto& type = param.type;
    if (!type.isLeaf()) {
      throw std::runtime_error("Not implemented");
    }
    auto typeStr = type.getLLVMString(true);
    paramsStr += sep + typeStr + " noundef %" + name;
    sep = ", ";
  }
  getHeader() += "define dso_local " + retType.getLLVMString(true) + " @" +
                 functionName + "(" + paramsStr + ") #0 {\n";
}

void LLVMGenerator::exitFunction() {
  getText() += "}\n\n";
  reg = 1;

  if (!activeFunctions.empty()) {
    activeFunctions.pop();
  }
  if (!functionDefs.empty()) {
    functionDefs.top()->ttl--;
    while (!functionDefs.empty() && functionDefs.top()->ttl == 0) {
      functionDefs.pop();
      functionDefs.top()->ttl--;
    }
  }
}

void LLVMGenerator::enterFunction() {
  reg = 1;
  auto node = std::make_unique<Node>();
  if (!functionDefs.empty()) {
    functionDefs.top()->neighbors.push_back(node.get());
  }
  functionDefs.push(node.get());
  activeFunctions.push(node.get());
  graph.insertNode(std::move(node));
}

void LLVMGenerator::createStruct(const Struct& struct_) {
  std::string structStr;
  structStr += "%struct." + struct_.name + " = type { ";
  const char* delim = "";
  for (const auto& field : struct_.fields) {
    structStr += delim + field.second.getLLVMString(false);
    delim = ", ";
  }
  structStr += " }";

  structsDeclarations += structStr + "\n";
}

void LLVMGenerator::createReturn(const types::Type& type,
                                 const std::string& arg) {
  getText() +=
      getIndent() + "ret " + type.getLLVMString(true) + " " + arg + "\n";
}

std::string LLVMGenerator::createCall(const std::string& functionName,
                                      const std::vector<FunctionParam>& args,
                                      const types::Type& retType) {
  std::string argsString;
  const char* sep = "";
  for (const auto& arg : args) {
    argsString += sep + arg.type.getLLVMString(true) + " noundef " + arg.name;
    sep = ", ";
  }
  auto callReg = getNumberedLabel("call", callRegisterNumber);
  getText() += getIndent() + "%" + callReg + " = call " +
               retType.getLLVMString(true) + " @" + functionName + "(" +
               argsString + ")\n";

  return "%" + callReg;
}

std::string LLVMGenerator::getIfThenLabel() {
  return getNumberedLabel("if.then", ifThenNumber);
}

std::string LLVMGenerator::getIfEndLabel() {
  return getNumberedLabel("if.end", ifEndNumber);
}

std::string LLVMGenerator::getIfElseLabel() {
  return getNumberedLabel("if.else", ifElseNumber);
}

std::string LLVMGenerator::getWhileCondLabel() {
  return getNumberedLabel("while.cond", whileCondNumber);
}

std::string LLVMGenerator::getWhileBodyLabel() {
  return getNumberedLabel("while.body", whileBodyNumber);
}

std::string LLVMGenerator::getWhileEndLabel() {
  return getNumberedLabel("while.end", whileEndNumber);
}

std::string LLVMGenerator::castI32ToDouble(const std::string& id) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = sitofp i32 " + id + " to double\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::castI32ToFloat(const std::string& id) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = sitofp i32 " + id + " to float\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::castDoubleToI32(const std::string& id) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = fptosi double " + id + " to i32\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::extBoolToI32(const std::string& id) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = zext i1 " + id + " to i32\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::truncateI8ToI1(const std::string& val) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = trunc i8 " + val + " to i1\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::truncI32ToBool(const std::string& val) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = trunc i32 " + val + " to i1\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::extI1toI8(const std::string& val) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = zext i1 " + val + " to i8\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::extI32toI64(const std::string& val) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = sext i32 " + val + " to i64\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::truncDoubleToFloat(const std::string& val) {
  const auto regStr = getRegStr();
  getText() +=
      getIndent() + regStr + " = fptrunc double " + val + " to float\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::castFloatToI32(const std::string& val) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = fptosi float " + val + " to i32\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::extFloatToDouble(const std::string& val) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = fpext float " + val + " to double\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::getElementPtrInBounds(const std::string& array,
                                                 const std::string& element,
                                                 const std::string& bounds,
                                                 bool isInternalPtr) {
  const auto arrayIdxStr = "%" + getNumberedLabel("arrayIdx", arrayIndexNumber);
  auto arrStr = (isInternalPtr ? "" : "%") + array;
  getText() += getIndent() + arrayIdxStr + " = getelementptr inbounds " +
               bounds + ", ptr " + arrStr + ", i64 0, i64 " + element + "\n";
  return arrayIdxStr;
}

void LLVMGenerator::printI32(const std::string& id) {
  getText() += getIndent() +
               "call i32 (ptr, ...) @printf(ptr noundef @formatIntNewLine, i32 "
               "noundef " +
               id + ")\n";
  ++reg;
}

void LLVMGenerator::printDouble(const std::string& id) {
  getText() +=
      getIndent() +
      "call i32 (ptr, ...) @printf(ptr noundef @formatFloatNewLine, double "
      "noundef " +
      id + ")\n";
  ++reg;
}

void LLVMGenerator::read(const std::string& id, const types::Type& type,
                         bool isInternalPtr) {
  if (!type.isLeaf()) {
    throw std::invalid_argument("Can't read non leaf types");
  }
  const auto& leafType = dynamic_cast<const types::LeafNode&>(type.getRoot());
  auto primitive = *leafType.getSimpleTypeId();
  std::string fStr;
  if (primitive == TypeManager::INT_TYPE_ID) {
    fStr = "@formatInt";
  } else if (primitive == TypeManager::DOUBLE_TYPE_ID) {
    fStr = "@formatDouble";
  } else if (primitive == TypeManager::FLOAT_TYPE_ID) {
    fStr = "@formatFloat";
  } else {
    throw std::invalid_argument("Not implemented for typeid " +
                                std::to_string(static_cast<int>(primitive)));
  }

  getText() += getIndent() + "call i32 (ptr, ...) @scanf(ptr noundef " + fStr +
               ", ptr noundef " + (isInternalPtr ? "" : "%") + id + ")\n";
  ++reg;
}

std::string LLVMGenerator::emitCode(const std::string& source_filename) {
  std::string code;
  code += "source_filename = \"" + source_filename + "\"\n";
  code +=
      "target triple = \"x86_64-w64-windows-gnu\"\n\n";  // TODO: obtain this somehow
  code += getPrintfFormatStrings() + "\n";
  code += getCStdLibDeclarations() + "\n";
  code += structsDeclarations + (structsDeclarations.empty() ? "" : "\n");
  TopologicalSorter sorter;
  auto orderedFunctions = sorter.getOrdered(graph);
  std::string functionDefinitions;
  for (auto it = orderedFunctions.rbegin(); it != orderedFunctions.rend();
       ++it) {
    const auto& funDef = *it;
    functionDefinitions += funDef->header + funDef->text;
  }
  code += functionDefinitions;
  code += "define dso_local i32 @main() #0 {\n";
  code += text;
  code += getIndent() + "ret i32 0\n}\n";
  return code;
}

std::string LLVMGenerator::getZeroLiteral(const types::Type& type) {
  if (!type.isLeaf())
    throw std::invalid_argument("Only for leafs");
  if (type.isInteral()) {
    return "0";
  }
  if (type.isFloatingPoint()) {
    return "0.0";
  }
  return "";
}

bool LLVMGenerator::supportsLiteralTranslation(const types::Type& from,
                                               const types::Type& to) {
  if (!from.isLeaf() || !to.isLeaf()) {
    return false;
  }
  const auto& fromLeaf = dynamic_cast<const types::LeafNode&>(from.getRoot());
  const auto& toLeaf = dynamic_cast<const types::LeafNode&>(to.getRoot());
  if (fromLeaf.getTypeID() == TypeManager::INT_TYPE_ID) {
    if (toLeaf.getTypeID() == TypeManager::DOUBLE_TYPE_ID ||
        toLeaf.getTypeID() == TypeManager::FLOAT_TYPE_ID) {
      return true;
    }
  }
  if (fromLeaf.getTypeID() == TypeManager::DOUBLE_TYPE_ID) {
    if (toLeaf.getTypeID() == TypeManager::FLOAT_TYPE_ID) {
      return true;
    }
  }
  return false;
}

std::string LLVMGenerator::getLiteral(const types::Type& from,
                                      const types::Type& to,
                                      const std::string& literal) {
  if (!from.isLeaf() || !to.isLeaf())
    throw std::invalid_argument("literals only for leafs");

  auto& fromLeaf = dynamic_cast<const types::LeafNode&>(from.getRoot());
  auto& toLeaf = dynamic_cast<const types::LeafNode&>(to.getRoot());
  if (fromLeaf.getTypeID() == TypeManager::INT_TYPE_ID) {
    if (toLeaf.getTypeID() == TypeManager::DOUBLE_TYPE_ID ||
        toLeaf.getTypeID() == TypeManager::FLOAT_TYPE_ID) {
      return literal + ".0";
    }
  }
  if (fromLeaf.getTypeID() == TypeManager::DOUBLE_TYPE_ID) {
    if (toLeaf.getTypeID() == TypeManager::FLOAT_TYPE_ID) {
      std::stringstream buffer;
      double x = std::stof(literal);
      buffer << std::hex << *reinterpret_cast<uint64_t*>(&x);
      return "0x" + buffer.str();
    }
  }
  throw std::runtime_error("Literal translation not supported");
}

std::string LLVMGenerator::getRegStr() const {
  return "%" + std::to_string(reg);
}

std::string LLVMGenerator::getPrintfFormatStrings() {
  std::string result;
  for (const auto& fStr : formatStrs) {
    result += fStr + "\n";
  }
  return result;
}

std::string LLVMGenerator::getCStdLibDeclarations() {
  std::string declarePrintf = "declare i32 @printf(ptr, ...)";
  std::string declareScanf = "declare i32 @scanf(ptr, ...)";
  return declarePrintf + "\n" + declareScanf + "\n";
}

std::string LLVMGenerator::getNumberedLabel(const char* label, int& num) {
  auto suffix = num == 0 ? "" : std::to_string(num);
  ++num;
  return label + suffix;
}
}  // namespace lexy2