#include "llvm_generator.hpp"

namespace lexy2 {
std::string LLVMGenerator::getTypeString(Type type) {
  switch (type) {
    case Type::I32:
      return "i32";
    case Type::I8:
      return "i8";
    case Type::DOUBLE:
      return "double";
    default:
      return "";
  }
}

std::string LLVMGenerator::getOpPrefix(Type type) {
  switch (type) {
    case Type::I32:
    case Type::I8:
      return "";
    case Type::DOUBLE:
      return "f";
    default:
      return "";
  }
}

std::string LLVMGenerator::getRelPrefix(Type type) {
  switch (type) {
    case Type::I32:
    case Type::I8:
      return "i";
    case Type::DOUBLE:
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
    case BinOpName::SREM:
      return "srem";
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
  if (type == Type::DOUBLE) {
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

std::string LLVMGenerator::castI32ToDouble(const std::string& id) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = sitofp i32 " + id + " to double\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::castDoubleToI32(const std::string& id) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = fptosi double " + id + " to i32\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::castBoolToI32(const std::string& id) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = zext i1 " + id + " to i32\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::truncateI8ToI1(const std::string& val) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = trunc i8 " + val + " to i1\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::castI1toI8(const std::string& val) {
  const auto regStr = getRegStr();
  text += getIndent() + regStr + " = zext i1 " + val + " to i8\n";
  reg++;
  return regStr;
}

void LLVMGenerator::printI32(const std::string& id) {
  text += getIndent() +
          "call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef " +
          id + ")\n";
  reg++;
}

void LLVMGenerator::printDouble(const std::string& id) {
  text +=
      getIndent() +
      "call i32 (ptr, ...) @printf(ptr noundef @formatDouble, double noundef " +
      id + ")\n";
  reg++;
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
      return "0.0";
    default:
      __builtin_unreachable();
  }
}
}  // namespace lexy2