#include "llvm_generator.hpp"

namespace lexy2 {
void LLVMGenerator::declareI32(const std::string& id) {
  text += "%" + id + " = alloca i32\n";
}

void LLVMGenerator::declareDouble(const std::string& id) {
  text += "%" + id + " = alloca double\n";
}

void LLVMGenerator::assignI32(const std::string& id, const std::string& value) {
  text += "store i32 " + value + ", i32* %" + id + "\n";
}

void LLVMGenerator::assignDouble(const std::string& id,
                                 const std::string& value) {
  text += "store double " + value + ", double* %" + id + "\n";
}

std::string LLVMGenerator::loadI32(const std::string& id) {
  auto regStr = getRegStr();
  text += regStr + " = load i32, i32* %" + id + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::loadDouble(const std::string& id) {
  auto regStr = getRegStr();
  text += regStr + " = load double, double* %" + id + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::addI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = add i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::addDouble(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fadd double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::subI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = sub i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::subDouble(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fsub double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::mulI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = mul i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::mulDouble(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fmul double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::divI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = sdiv i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::divDouble(const std::string& val1,
                                     const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = fdiv double " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::remI32(const std::string& val1,
                                  const std::string& val2) {
  const auto regStr = getRegStr();
  text += regStr + " = srem i32 " + val1 + ", " + val2 + "\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::castI32ToDouble(const std::string& id) {
  const auto regStr = getRegStr();
  text += regStr + " = sitofp i32 " + id + " to double\n";
  reg++;
  return regStr;
}

std::string LLVMGenerator::castDoubleToI32(const std::string& id) {
  const auto regStr = getRegStr();
  text += regStr + " = fptosi double " + id + " to i32\n";
  reg++;
  return regStr;
}

void LLVMGenerator::printI32(const std::string& id) {
  text += "call i32 (ptr, ...) @printf(ptr noundef @formatInt, i32 noundef " +
          id + ")\n";
  reg++;
}

void LLVMGenerator::printDouble(const std::string& id) {
  text +=
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
  code += "ret i32 0\n}\n";
  return code;
}
}  // namespace lexy2