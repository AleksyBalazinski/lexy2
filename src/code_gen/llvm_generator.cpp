#include "code_gen/llvm_generator.hpp"
#include <sstream>
#include "code_gen/topo_sorter.hpp"
#include "function_param.hpp"
#include "types/llvm_str_visitor.hpp"
#include "types/type.hpp"
#include "utils.hpp"

namespace lexy2 {
std::string LLVMGenerator::getTypeString(Type type) {
  switch (type) {
    case Type::I32:
      return "i32";
    case Type::I8:
      return "i8";
    case Type::DOUBLE:
      return "double";
    case Type::FLOAT:
      return "float";
    case Type::I1:
      return "i1";
    default:
      return "";
  }
}

std::string LLVMGenerator::getOpPrefix(Type type, BinOpName op) {
  if (op != BinOpName::DIV && op != BinOpName::REM) {
    switch (type) {
      case Type::I32:
      case Type::I8:
        return "";
      case Type::DOUBLE:
      case Type::FLOAT:
        return "f";
      default:
        return "";
    }
  } else {
    switch (type) {
      case Type::I32:
      case Type::I8:
        return "s";
      case Type::DOUBLE:
      case Type::FLOAT:
        return "f";
      default:
        return "";
    }
  }
}

std::string LLVMGenerator::getRelPrefix(Type type) {
  switch (type) {
    case Type::I32:
    case Type::I8:
      return "i";
    case Type::DOUBLE:
    case Type::FLOAT:
      return "f";
    default:
      return "";
  }
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
    default:
      return "";
  }
}

std::string LLVMGenerator::getRelName(RelName relName) {
  switch (relName) {
    case RelName::EQ:
      return "eq";
    case RelName::NE:
      return "ne";
    case RelName::GE:
      return "ge";
    case RelName::GT:
      return "gt";
    case RelName::LE:
      return "le";
    case RelName::LT:
      return "lt";
    default:
      return "";
  }
}

std::string LLVMGenerator::getRelNamePrefix(RelName relName, Type type) {
  if (type == Type::DOUBLE || type == Type::FLOAT) {
    return "o";
  }
  if (type == Type::I32) {
    if (relName == RelName::EQ || relName == RelName::NE)
      return "";
    else
      return "s";
  }
  throw std::runtime_error("Not implemented");
}

std::string LLVMGenerator::getIndent() {
  return "  ";
}

std::string LLVMGenerator::createBinOp(BinOpName op, Type type,
                                       const std::string& arg1,
                                       const std::string& arg2) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = " + getOpPrefix(type, op) +
               getOperationString(op) + " " + getTypeString(type) + " " + arg1 +
               ", " + arg2 + "\n";
  ++reg;
  return regStr;
}
std::string LLVMGenerator::createRel(Type type, RelName relName,
                                     const std::string& arg1,
                                     const std::string& arg2) {
  const auto regStr = getRegStr();
  getText() += getIndent() + regStr + " = " + getRelPrefix(type) + "cmp" + " " +
               getRelNamePrefix(relName, type) + getRelName(relName) + " " +
               getTypeString(type) + " " + arg1 + ", " + arg2 + "\n";
  ++reg;
  return regStr;
}

void LLVMGenerator::createAssignment(Type type, const std::string& identifier,
                                     const std::string& value,
                                     bool isInternalPtr) {
  std::string idString = (isInternalPtr ? "" : "%") + identifier;
  getText() += getIndent() + "store " + getTypeString(type) + " " + value +
               ", " + getTypeString(type) + "* " + idString + "\n";
}

void LLVMGenerator::createCustomAssignment(const std::string& typeStr,
                                           const std::string& identifier,
                                           const std::string& value,
                                           bool isInternalPtr) {
  std::string idString = (isInternalPtr ? "" : "%") + identifier;
  getText() += getIndent() + "store " + typeStr + " " + value + ", " + typeStr +
               "* " + idString + "\n";
}

void LLVMGenerator::createDeclaration(Type type, const std::string& arg) {
  getText() +=
      getIndent() + "%" + arg + " = alloca " + getTypeString(type) + "\n";
}

void LLVMGenerator::createCustomDeclaration(const std::string& typeString,
                                            const std::string& arg) {
  getText() += getIndent() + "%" + arg + " = alloca " + typeString + "\n";
}

std::string LLVMGenerator::createLoad(Type type, const std::string& id,
                                      bool isInternalPtr) {
  const auto regStr = getRegStr();
  std::string idString = (isInternalPtr ? "" : "%") + id;
  getText() += getIndent() + regStr + " = load " + getTypeString(type) + ", " +
               getTypeString(type) + "* " + idString + "\n";
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
                                   Type retType) {
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
  getHeader() += "define dso_local " + getTypeString(retType) + " @" +
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

void LLVMGenerator::createReturn(Type type, const std::string& arg) {
  getText() += getIndent() + "ret " + getTypeString(type) + " " + arg + "\n";
}

std::string LLVMGenerator::createCall(const std::string& functionName,
                                      const std::vector<FunctionParam>& args,
                                      Type retType) {
  std::string argsString;
  const char* sep = "";
  for (const auto& arg : args) {
    argsString += sep + arg.type.getLLVMString(true) + " noundef " + arg.name;
    sep = ", ";
  }
  auto callReg = getNumberedLabel("call", callRegisterNumber);
  getText() += getIndent() + "%" + callReg + " = call " +
               getTypeString(retType) + " @" + functionName + "(" + argsString +
               ")\n";

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
  getText() +=
      getIndent() +
      "call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef " + id +
      ")\n";
  ++reg;
}

void LLVMGenerator::printDouble(const std::string& id) {
  getText() += getIndent() +
               "call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double "
               "noundef " +
               id + ")\n";
  ++reg;
}

std::string LLVMGenerator::emitCode(const std::string& source_filename) {
  std::string code;
  code += "source_filename = \"" + source_filename + "\"\n";
  code +=
      "target triple = \"x86_64-w64-windows-gnu\"\n\n";  // TODO: obtain this somehow
  code += getPrintfFormatStrings() + "\n";
  code += getCStdLibDeclarations() + "\n";
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

std::string LLVMGenerator::getZeroLiteral(Type type) {
  switch (type) {
    case Type::I32:
    case Type::I8:
      return "0";
    case Type::DOUBLE:
    case Type::FLOAT:
      return "0.0";
    default:
      __builtin_unreachable();
  }
}

bool LLVMGenerator::supportsLiteralTranslation(Type from, Type to) {
  if (from == Type::I32) {
    if (to == Type::DOUBLE || to == Type::FLOAT) {
      return true;
    }
  }
  if (from == Type::DOUBLE) {
    if (to == Type::FLOAT) {
      return true;
    }
  }
  return false;
}

std::string LLVMGenerator::getLiteral(Type from, Type to,
                                      const std::string& literal) {
  if (from == Type::I32) {
    if (to == Type::DOUBLE || to == Type::FLOAT) {
      return literal + ".0";
    }
  }
  if (from == Type::DOUBLE) {
    if (to == Type::FLOAT) {
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
  std::string formatInt =
      R"(@formatInt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1)";
  std::string formatDouble =
      R"(@formatDouble = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1)";
  return formatInt + "\n" + formatDouble + "\n";
}

std::string LLVMGenerator::getCStdLibDeclarations() {
  std::string declarePrintf = "declare i32 @printf(ptr, ...)";
  return declarePrintf + "\n";
}

std::string LLVMGenerator::getNumberedLabel(const char* label, int& num) {
  auto suffix = num == 0 ? "" : std::to_string(num);
  ++num;
  return label + suffix;
}
}  // namespace lexy2