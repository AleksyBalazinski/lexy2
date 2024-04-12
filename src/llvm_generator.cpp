#include "llvm_generator.hpp"
#include <sstream>
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
  text += getIndent() + regStr + " = " + getOpPrefix(type, op) +
          getOperationString(op) + " " + getTypeString(type) + " " + arg1 +
          ", " + arg2 + "\n";
  ++reg;
  return regStr;
}
std::string LLVMGenerator::createRel(Type type, RelName relName,
                                     const std::string& arg1,
                                     const std::string& arg2) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = " + getRelPrefix(type) + "cmp" + " " +
          getRelNamePrefix(relName, type) + getRelName(relName) + " " +
          getTypeString(type) + " " + arg1 + ", " + arg2 + "\n";
  ++reg;
  return regStr;
}

void LLVMGenerator::createAssignment(Type type, const std::string& identifier,
                                     const std::string& value) {
  text += getIndent() + "store " + getTypeString(type) + " " + value + ", " +
          getTypeString(type) + "* %" + identifier + "\n";
}

void LLVMGenerator::createDeclaration(Type type, const std::string& arg) {
  text += getIndent() + "%" + arg + " = alloca " + getTypeString(type) + "\n";
}

std::string LLVMGenerator::createLoad(Type type, const std::string& id) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = load " + getTypeString(type) + ", " +
          getTypeString(type) + "* %" + id + "\n";
  ++reg;
  return regStr;
}

void LLVMGenerator::createBranch(const std::string& cond,
                                 const std::string& ifTrue,
                                 const std::string& ifFalse) {
  text += getIndent() + "br i1 " + cond + ", label %" + ifTrue + ", label %" +
          ifFalse + "\n";
}

void LLVMGenerator::createBranch(const std::string& dest) {
  text += getIndent() + "br label %" + dest + "\n";
}

void LLVMGenerator::createLabel(const std::string& label) {
  text += label + ":\n";
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
  text += getIndent() + regStr + " = sitofp i32 " + id + " to double\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::castI32ToFloat(const std::string& id) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = sitofp i32 " + id + " to float\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::castDoubleToI32(const std::string& id) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = fptosi double " + id + " to i32\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::extBoolToI32(const std::string& id) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = zext i1 " + id + " to i32\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::truncateI8ToI1(const std::string& val) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = trunc i8 " + val + " to i1\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::extI1toI8(const std::string& val) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = zext i1 " + val + " to i8\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::truncDoubleToFloat(const std::string& val) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = fptrunc double " + val + " to float\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::castFloatToI32(const std::string& val) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = fptosi float " + val + " to i32\n";
  ++reg;
  return regStr;
}

std::string LLVMGenerator::extFloatToDouble(const std::string& val) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = fpext float " + val + " to double\n";
  ++reg;
  return regStr;
}

void LLVMGenerator::printI32(const std::string& id) {
  text += getIndent() +
          "call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef " +
          id + ")\n";
  ++reg;
}

void LLVMGenerator::printDouble(const std::string& id) {
  text += getIndent() +
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